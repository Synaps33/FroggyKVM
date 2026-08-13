#include "incls/_precompiled.incl"

void ObjectHeap::do_nothing(OopDesc** p) { (void)p; }
void ObjectHeap::mark_pointer_to_young_generation(OopDesc** p) { (void)p; }
void ObjectHeap::mark_root_and_stack(OopDesc** p) { (void)p; }

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

void OopDesc::oops_do_for(const FarClassDesc* f, void (*fn)(OopDesc**)) { (void)f; (void)fn; }
void ObjectHeap::continue_marking() {}

ConstantTag ConstantPool::tag_at(int index) const  {
  TypeArray::Raw ta = tags();
  jubyte* ptr = (jubyte*)ta().base_address();
  return ConstantTag(ptr[index]);
}
