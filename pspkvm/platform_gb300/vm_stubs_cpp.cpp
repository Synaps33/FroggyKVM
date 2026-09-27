#include "incls/_precompiled.incl"

void oop_write_barrier(OopDesc** addr, OopDesc* value) {
  OopDesc ** heap_start = _heap_start;
  OopDesc ** old_generation_end = _old_generation_end;
  *addr = value;

  // Note the order of the comparison. In most cases the first comparison 
  // will fail because addr is in the young space
  if (addr < old_generation_end && ((OopDesc*)addr) < value && heap_start <= addr) {
    ObjectHeap::set_bit_for(addr);
    GUARANTEE(ObjectHeap::test_bit_for(addr), "sanity check");
  }
}

void ObjectHeap::do_nothing(OopDesc** p) { (void)p; }
void ObjectHeap::mark_pointer_to_young_generation(OopDesc** p) {
  if (p < _collection_area_start) {
    OopDesc** const obj = (OopDesc**)*p;
    if (_collection_area_start <= obj && obj < _inline_allocation_top) {
      set_bit_for(p);
    }
  }
}

void ObjectHeap::mark_root_and_stack(OopDesc** p) {
  OopDesc** const obj = (OopDesc**) *p;
  if (_collection_area_start <= obj && obj < mark_area_end()
      && !test_and_set_bit_for(obj)) {
    *_marking_stack_top++ = (OopDesc*)obj;
    continue_marking();
  }
}

void ObjectHeap::mark_and_push(OopDesc** p) {
  OopDesc* obj = *p;
  if (_collection_area_start <= (OopDesc**)obj &&
      (OopDesc**)obj < mark_area_end()) {
    if (!test_and_set_bit_for((OopDesc**)obj)) {
      if (_marking_stack_top == _marking_stack_end) {
        _marking_stack_overflow = true;
      } else {
        *_marking_stack_top++ = obj;
      }
    }
  }
}

void ObjectHeap::continue_marking(void) {
  OopDesc** const collection_area_start = _collection_area_start;
  OopDesc** const heap_top              = mark_area_end();
  OopDesc** const marking_stack_end     = _marking_stack_end;
  address   const bitvector_base        = _bitvector_base;

  while (_marking_stack_top > _marking_stack_start) {
    OopDesc* obj = *--_marking_stack_top;
    mark_and_push(&(obj->_klass));

    FarClassDesc* const blueprint = obj->blueprint();
    if (blueprint->instance_size_as_jint() > 0) {
      const jbyte* map = (jbyte*)blueprint->embedded_oop_map();
      OopDesc** p = (OopDesc**)obj;
      for (;;) {
        const jint entry = (jint)(*map++);
        if (entry > 0) {
          p += entry;
          OopDesc** const o = (OopDesc**)*p;
          if (collection_area_start <= o && o < heap_top) {
            if (!test_and_set_bit_for(o, bitvector_base)) {
              if (_marking_stack_top == marking_stack_end) {
                _marking_stack_overflow = true;
              } else {
                *_marking_stack_top++ = (OopDesc*)o;
              }
            }
          }
        } else if (entry == 0) {
          break;
        } else {
          GUARANTEE((entry & 0xff) == OopMapEscape, "sanity")
          p += (OopMapEscape - 1);
        }
      }
    } else {
      obj->oops_do_for(blueprint, mark_and_push);
    }
  }
}

#if ENABLE_ISOLATES
void TaskMirrorDesc::variable_oops_do(void do_oop(OopDesc **)) {
  if (_object_size == header_size()) {
    return;
  }
  if (_containing_class != NULL) {
    jubyte *map = _containing_class->embedded_oop_map();
    while (*map++ != OopMapSentinel) {}
    if (*map != OopMapSentinel) {
      map_oops_do(map, do_oop);
    }
  }
}
#endif

void ConstantPoolDesc::variable_oops_do(void do_oop(OopDesc**)) {
  if (_tags == NULL) {
    return;
  }
  OopDesc** base = (OopDesc**)((jubyte*)this + header_size());
  jubyte* tags = (jubyte*)_tags + sizeof(ArrayDesc);
  for (int i = 0; i < _length; i++) {
    jubyte tag = *tags++;
    if (ConstantTag::is_oop(tag)) {
      GUARANTEE(*base != NULL, "constant pool cannot contain null entries");
      do_oop(base);
    }
    base++;
  }
}

#if !ENABLE_ISOLATES 
void InstanceClassDesc::variable_oops_do(void do_oop(OopDesc**)) {
  jubyte* map = embedded_oop_map();
  while (*map++ != OopMapSentinel) {};
  map_oops_do(map, do_oop);
}
#else
typedef void dummy_instance_class_function(OopDesc**);
void InstanceClassDesc::variable_oops_do(dummy_instance_class_function) {
}
#endif

void MixedOopDesc::variable_oops_do(void do_oop(OopDesc**)) {
  OopDesc** addr = obj_field_addr(sizeof(MixedOopDesc));
  for (int i=0; i<_pointer_count; i++) {
    do_oop(addr);
    addr ++;
  }
}

void EntryActivationDesc::variable_oops_do(void do_oop(OopDesc**)) {
  for (int i = 0; i < _length; i++) {
    if (tag_at(i) == obj_tag) {
      do_oop(pointer_to_value_at(i));
    }
  }
}

void ConstantPool::resolve_helper_0(int index, Symbol* name, Symbol* signature,
                                    InstanceClass* klass, Symbol* klass_name
                                    JVM_TRAPS) {
  int value, name_and_type_value;
  jushort name_and_type_index, name_index, signature_index, class_index;
  jushort len = length();

  {
    AllocationDisabler shouldnt_allocate_in_this_block;
    TypeArray::Raw ta = tags();
    jubyte *tag = (jubyte*)ta().base_address();
    if (!is_within_bounds(index, len) ||
        !ConstantTag::is_field_or_method(tag[index])) {
      printf("[GB300-DEBUG] resolve_helper_0 err 1: index=%d out of bounds or not field/method. tag=%d\n", index, tag[index]);
      goto error;
    }
    value = int_field(offset_from_index(index));
    name_and_type_index = extract_high_jushort_from_jint(value);
    class_index         = extract_low_jushort_from_jint (value);
    if (!is_within_bounds(name_and_type_index, len) ||
        !ConstantTag::is_name_and_type(tag[name_and_type_index])) {
      printf("[GB300-DEBUG] resolve_helper_0 err 2: name_and_type_index=%d out of bounds or not name_and_type. tag=%d\n", name_and_type_index, tag[name_and_type_index]);
      goto error;
    }
    if (!is_within_bounds(class_index, len) || 
        !ConstantTag::is_klass(tag[class_index])) {
      printf("[GB300-DEBUG] resolve_helper_0 err 3: class_index=%d out of bounds or not klass. tag=%d\n", class_index, tag[class_index]);
      goto error;
    }
    name_and_type_value = int_field(offset_from_index(name_and_type_index));
    name_index      = extract_low_jushort_from_jint (name_and_type_value);
    signature_index = extract_high_jushort_from_jint(name_and_type_value);
    if (!is_within_bounds(name_index, len) ||
        !ConstantTag::is_utf8(tag[name_index])) {
      printf("[GB300-DEBUG] resolve_helper_0 err 4: name_index=%d out of bounds or not utf8. tag=%d\n", name_index, tag[name_index]);
      goto error;
    }
    if (!is_within_bounds(signature_index, len) ||
        !ConstantTag::is_utf8(tag[signature_index])) {
      printf("[GB300-DEBUG] resolve_helper_0 err 5: sig_index=%d out of bounds or not utf8. tag=%d\n", signature_index, tag[signature_index]);
      goto error;
    }
    *name = symbol_at(name_index);
    *signature = symbol_at(signature_index);
  }

  if (ConstantTag::is_unresolved_klass(tag_value_at(class_index))) {
    if (klass != NULL) {
      *klass = klass_at(class_index JVM_NO_CHECK_AT_BOTTOM);
    } else {
      *klass_name = unchecked_unresolved_klass_at(class_index);
    }
  } else {
    GUARANTEE(ConstantTag::is_resolved_klass(tag_value_at(class_index)), 
              "sanity");
    jint class_id = int_field(offset_from_index(class_index));
    JavaClass::Raw k = Universe::class_from_id(class_id);
    if (klass != NULL) {
      *klass = k.obj();
    } else {
      *klass_name = k().name();
    }
  }
  return;
error:
  Throw::error(invalid_constant JVM_NO_CHECK_AT_BOTTOM);
}

int ConstantPool::name_and_type_ref_index_at(int index JVM_TRAPS) {
  TypeArray::Raw ta = tags();
  jubyte *tag_base = (jubyte*)ta().base_address();
  jint   *val_base = (jint*) ( (int)(obj()) + base_offset() );
  int len = length();
  jint ref_index, name_and_type_index;

  if ((juint)index >= (juint)len) {
    printf("[GB300-DEBUG] name_and_type_ref_index_at error 1: index=%d >= len=%d\n", index, len);
    goto error;
  }
  ref_index = val_base[index];
  {
    ConstantTag tag1(tag_base[index]);
    if (!tag1.is_field_or_method()) {
      printf("[GB300-DEBUG] name_and_type_ref_index_at error 2: tag1=%d is not field/method at index=%d\n", tag1.value(), index);
      goto error;
    }
  }
  name_and_type_index = extract_high_jshort_from_jint(ref_index);
  if ((juint)name_and_type_index >= (juint)len) {
    printf("[GB300-DEBUG] name_and_type_ref_index_at error 3: name_and_type_index=%d >= len=%d\n", name_and_type_index, len);
    goto error;
  }
  {
    ConstantTag tag2(tag_base[name_and_type_index]);
    if (!tag2.is_name_and_type()) {
      printf("[GB300-DEBUG] name_and_type_ref_index_at error 4: tag2=%d is not name_and_type at name_and_type_index=%d\n", tag2.value(), name_and_type_index);
      goto error;
    }
  }
  GUARANTEE(name_and_type_index != 0, "sanity for JVM_ZCHECK");
  return name_and_type_index;
error:
  Throw::error(invalid_constant JVM_NO_CHECK_AT_BOTTOM);
  return 0;
}

int Field::find_field_index(InstanceClass* ic, Symbol* name, Symbol* signature)
{
  AllocationDisabler shouldnt_allocate_in_this_function;
  ConstantPool::Raw cp = get_constants_for(ic);
  TypeArray::Raw fields = get_fields_for(ic);
  int fields_length = fields().length();
  OopDesc *name_obj = name->obj();
  OopDesc *sig_obj  = signature->obj();
  address field_base = fields().base_address();
  address cp_base = ((address)cp.obj()) + ConstantPool::base_offset();
  for (int index = 0; index < fields_length; index += 5) {
    int name_index      = ((jushort*)field_base)[NAME_OFFSET];
    int signature_index = ((jushort*)field_base)[SIGNATURE_OFFSET];
    OopDesc *n = ((OopDesc**)cp_base)[name_index];
    OopDesc *s = ((OopDesc**)cp_base)[signature_index];
    if (n == name_obj && s == sig_obj) {
      return index;
    }
    field_base += 5 * sizeof(jushort);
  }
  return -1;
}

int Bytecodes::length_for(const Method* method, const int bci) {
  const Code code = method->bytecode_at(bci);
  check(code);
  const int size = length_for(code);
  return size ? size : wide_length_for(method, bci, code);
}

juint Inflater::crc32(const unsigned char* b, unsigned int len) {
  juint crc = 0xFFFFFFFF;
  const unsigned char* end = b + len;
  for ( ; b < end; b++) {
    crc ^= *b;
    for (unsigned int j = 8; j > 0; --j) {
      crc = (crc & 1) ? ((crc >> 1) ^ 0xedb88320) : (crc >> 1);
    }
  }
  return ~crc;
}

int VerifierFrame::get_stackmap_index_for_offset(int target_bci) {
  int len = stackmaps()->length();
  address *p = (address*)stackmaps()->base_address();
  int stackmap_index = 0;
  while (stackmap_index < len) {
    address scalars = *p;
    scalars += Array::base_offset();
    if (target_bci == ((jint*)scalars)[0]) {
      return stackmap_index;
    }
    p += 2;
    stackmap_index += 2;
  }
  return -1;
}

void OopDesc::oops_do_for(const FarClassDesc* blueprint, void do_oop(OopDesc**)) {
  jint instance_size = blueprint->instance_size_as_jint();
  switch(instance_size) {
  default:
    GUARANTEE(instance_size > 0, "bad instance size");
    map_oops_do(blueprint->embedded_oop_map(), do_oop);
    return;
    
  case InstanceSize::size_type_array_1:
  case InstanceSize::size_type_array_2:
  case InstanceSize::size_type_array_4:
  case InstanceSize::size_type_array_8:
  case InstanceSize::size_generic_near:
  case InstanceSize::size_symbol:
    GUARANTEE(blueprint->extern_oop_map()[0] == OopMapSentinel, 
              "No fixed pointers");
    return;
    
  case InstanceSize::size_far_class:
  case InstanceSize::size_obj_near: 
  case InstanceSize::size_java_near:
  case InstanceSize::size_boundary:
  case InstanceSize::size_method:
    break;

  case InstanceSize::size_obj_array_class:
  case InstanceSize::size_type_array_class: 
    break;
    
  case InstanceSize::size_execution_stack:
    ((ExecutionStackDesc*) this)->variable_oops_do(do_oop); 
    GUARANTEE(blueprint->extern_oop_map()[0] == OopMapSentinel, 
              "No fixed pointers");
    return;

  case InstanceSize::size_obj_array:
    ((ObjArrayDesc*) this)->variable_oops_do(do_oop); 
    GUARANTEE(blueprint->extern_oop_map()[0] == OopMapSentinel, 
              "No fixed pointers");
    return;

  case InstanceSize::size_refnode:
#if ENABLE_JAVA_DEBUGGER
    ((RefNodeDesc *) this)->variable_oops_do(do_oop); 
    GUARANTEE(blueprint->extern_oop_map()[0] == OopMapSentinel, 
              "No fixed pointers");
#endif
    return;

  case InstanceSize::size_mixed_oop:
    ((MixedOopDesc*) this)->variable_oops_do(do_oop); 
    break;
#if USE_COMPILER_STRUCTURES
  case InstanceSize::size_compiled_method:
    ((CompiledMethodDesc*) this)->variable_oops_do(do_oop); 
    break;
#endif
  case InstanceSize::size_constant_pool: 
    ((ConstantPoolDesc*) this)->variable_oops_do(do_oop);  
    break;
  case InstanceSize::size_entry_activation:  
    ((EntryActivationDesc*)this)->variable_oops_do(do_oop);
    break;
  case InstanceSize::size_instance_class:
    ((InstanceClassDesc*) this)->variable_oops_do(do_oop);
    break;
  case InstanceSize::size_class_info:
    ((ClassInfoDesc*) this)->variable_oops_do(do_oop);  
    break;
  case InstanceSize::size_stackmap_list:
    ((StackmapListDesc*) this)->variable_oops_do(do_oop);  
    break;
#if ENABLE_ISOLATES
  case InstanceSize::size_task_mirror:
    ((TaskMirrorDesc*) this)->variable_oops_do(do_oop);  
    break;
#endif
  } 
  map_oops_do(blueprint->extern_oop_map(), do_oop);
}

ConstantTag ConstantPool::tag_at(int index) const  {
  TypeArray::Raw ta = tags();
  jubyte* ptr = (jubyte*)ta().base_address();
  return ConstantTag(ptr[index]);
}

#include <midpServices.h>
#include <midp_thread.h>

extern "C" {
jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_poll0() {
  midp_thread_wait(PUSH_SIGNAL, 0, 0);
  return -1;
}

jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_getEntry0() {
  return -1;
}

jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_getMIDlet0() {
  return -1;
}

jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_list0() {
  return -1;
}

jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_del0() {
  return -1;
}

jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_add0() {
  return -1;
}

jlong Java_com_sun_midp_io_j2me_push_ConnectionRegistry_addAlarm0() {
  return 0;
}

jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_checkInByName0() {
  return -1;
}

void Java_com_sun_midp_io_j2me_push_ConnectionRegistry_checkInByHandle0() {
}

void Java_com_sun_midp_io_j2me_push_ConnectionRegistry_checkInByMidlet0() {
}

void Java_com_sun_midp_io_j2me_push_ConnectionRegistry_delAllForSuite0() {
}
}

