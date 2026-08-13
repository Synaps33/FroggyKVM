// Modified Throw.cpp with debug logging for class_format_error and error
// This replaces the original Throw.o from the pre-built library

#include "incls/_precompiled.incl"
#include "incls/_Throw.cpp.incl"

ReturnOop
Throw::new_exception(Symbol* class_name, String* message JVM_TRAPS) {
  UsingFastOops fast_oops;

  GUARANTEE(!CURRENT_HAS_PENDING_EXCEPTION, "No pending exceptions");
  Thread::clear_current_pending_exception();

#if !defined(PRODUCT) || ENABLE_TTY_TRACE
  if (TraceExceptions || GenerateROMImage) {
    tty->print("TraceExceptions: ");
    class_name->print_value_on(tty);
    if (message->not_null()) {
      tty->print(" msg: ");
      message->print_value_on(tty);
    }
    tty->cr();
  }
  if (TraceExceptions) {
    ps();
  }
#endif
  if (Universe::is_stopping()) {
     return Universe::string_class()->obj();
  }
  InstanceClass::Fast exception_class =
      SystemDictionary::resolve(class_name, ErrorOnFailure JVM_NO_CHECK);
  if (CURRENT_HAS_PENDING_EXCEPTION) {
    GUARANTEE(exception_class.is_null(), "Sanity check");
    UsingFastOops raw_internal;
    Thread::clear_current_pending_exception();
    SymbolStream s1(class_name);
#ifndef PRODUCT
    String::Fast err_message;
    if (message->is_null()) {
      err_message = Universe::new_string(&s1 JVM_CHECK_0);
    } else {
      UsingFastOops raw_internal2;
      LiteralStream s2(": ", 0, jvm_strlen(": "));
      TypeArray::Fast message_char_array = message->value();
      CharStream s3(&message_char_array);
      ConcatenatedStream c2(&s1, &s2);
      ConcatenatedStream c3(&c2, &s3);
      err_message = Universe::new_string(&c3 JVM_CHECK_0);
    }
#else
    String::Fast err_message = Universe::new_string(&s1 JVM_CHECK_0);
#endif
    return new_exception(Symbols::java_lang_Error(),
                         &err_message JVM_NO_CHECK_AT_BOTTOM);
  }

  GUARANTEE(!exception_class.is_null(), "Sanity check");
  exception_class().initialize(JVM_SINGLE_ARG_CHECK_0);

  Throwable::Fast exception = Universe::new_instance(&exception_class 
                                                     JVM_CHECK_0);

  EntryActivation::Fast entry;
  Method::Fast init;
  init = exception_class().lookup_method(Symbols::object_initializer_name(),
      message->is_null() ? Symbols::void_signature() : Symbols::string_void_signature());
  entry = Universe::new_entry_activation(&init, message->is_null() ? 1 : 2 JVM_CHECK_0);
  if (!message->is_null()) {
    entry().obj_at_put(1, message);
    exception().set_message(message);
  }
  entry().obj_at_put(0, &exception);
  Thread::current()->append_pending_entry(&entry);
  return exception;
}

void Throw::allocate_and_throw(Symbol* class_name, String* message JVM_TRAPS) {
  GUARANTEE(!CURRENT_HAS_PENDING_EXCEPTION, "No pending exceptions");
  Thread::clear_current_pending_exception();

  OopDesc* exception = new_exception(class_name, message JVM_NO_CHECK);
  if (exception) {
    Thread::set_current_pending_exception(exception);
  } else {
    GUARANTEE(CURRENT_HAS_PENDING_EXCEPTION, 
              "out-of-memory should have been thrown");
  }
}

void Throw::allocate_and_throw(Symbol* class_name, char *non_cldc_class_name,
                               String* message JVM_TRAPS) {
  UsingFastOops fast_oops;
  String::Fast msg = add_message_prefix(non_cldc_class_name, message 
                                        JVM_MUST_SUCCEED);
  allocate_and_throw(class_name, &msg JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::allocate_and_throw(Symbol* class_name, char *non_cldc_class_name,
                               ErrorMsgTag err JVM_TRAPS) {
  UsingFastOops fast_oops;
  GUARANTEE(!CURRENT_HAS_PENDING_EXCEPTION, "No pending exceptions");
  Thread::clear_current_pending_exception();

  String::Fast str;
  char *message = ErrorMessage::get(err);
  if (message != NULL && message[0] != 0) {
    str = Universe::new_string(message, jvm_strlen(message) JVM_NO_CHECK);
    Thread::clear_current_pending_exception();
  }
  if (non_cldc_class_name != NULL) {
    str = add_message_prefix(non_cldc_class_name, &str JVM_MUST_SUCCEED);
  }

  Throwable::Fast exception = new_exception(class_name, &str JVM_NO_CHECK);
  if (exception.not_null()) {
    Thread::set_current_pending_exception(&exception);
  } else {
    GUARANTEE(CURRENT_HAS_PENDING_EXCEPTION, 
              "out-of-memory should have been thrown");
  }
}

ReturnOop
Throw::allocate_exception(Symbol* class_name, String* message JVM_TRAPS) {
  GUARANTEE(!CURRENT_HAS_PENDING_EXCEPTION, "No pending exceptions");
  Thread::clear_current_pending_exception();
  return new_exception(class_name, message JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::error(ErrorMsgTag err JVM_TRAPS) {
  void* ra = __builtin_return_address(0);
  printf("[GB300-DEBUG] Throw::error called! err=%d, caller=%p\n", err, ra);
  allocate_and_throw(Symbols::java_lang_Error(), err JVM_NO_CHECK_AT_BOTTOM);
}

#define error_template(tag, msg) msg,
static const char* const g_error_msgs[] = {
  ERROR_MESSAGES_DO(error_template)
};
#undef error_template

void Throw::class_format_error(ErrorMsgTag err JVM_TRAPS) {
  void* ra = __builtin_return_address(0);
  const char* err_str = (err >= 0 && err < (int)(sizeof(g_error_msgs)/sizeof(char*))) ? g_error_msgs[err] : "UNKNOWN";
  printf("[GB300-DEBUG] Throw::class_format_error called! err=%d, msg='%s', caller=%p\n", err, err_str, ra);
  allocate_and_throw(Symbols::java_lang_ClassFormatError(),
                     (char*)"ClassFormatError", err
                     JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::verify_error(ErrorMsgTag err JVM_TRAPS) {
  void* ra = __builtin_return_address(0);
  const char* err_str = (err >= 0 && err < (int)(sizeof(g_error_msgs)/sizeof(char*))) ? g_error_msgs[err] : "UNKNOWN";
  printf("[GB300-DEBUG] Throw::verify_error called! err=%d, msg='%s', caller=%p\n", err, err_str, ra);
  allocate_and_throw(Symbols::java_lang_VerifyError(), 
                     (char*)"VerifyError", err 
                     JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::out_of_memory_error(JVM_SINGLE_ARG_TRAPS) {
  JVM_IGNORE_TRAPS;
#if !defined(PRODUCT) || ENABLE_TTY_TRACE
  if (TraceExceptions) {
    TTY_TRACE_CR(("TraceExceptions: OutOfMemoryError"));
    ps();
  }
#endif

  Thread::set_current_pending_exception(
      Universe::out_of_memory_error_instance());
  printf("[FATAL] OutOfMemoryError thrown!\n");
}


void Throw::class_not_found(Symbol* class_name, FailureMode fail_mode 
                            JVM_TRAPS) {
  LoaderContext loader_ctx(class_name, fail_mode);
  Throw::class_not_found(&loader_ctx JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::class_not_found(LoaderContext *loader_ctx JVM_TRAPS) {
  if (Universe::before_main()) {
     TTY_TRACE(("class not found: "));
     loader_ctx->class_name()->print_symbol_on(tty);
     tty->cr();
     JVM_FATAL(main_class_not_found);
  }
  UsingFastOops fast_oops;
  String::Fast str = Universe::new_string(loader_ctx->class_name() JVM_CHECK);
  Throwable::Fast error;
  if (loader_ctx->fail_mode() == ErrorOnFailure) {
#if !ENABLE_CLDC_11
    str = add_message_prefix("NoClassDefFoundError", &str JVM_MUST_SUCCEED);
#endif
    error = allocate_exception(Symbols::java_lang_NoClassDefFoundError(), &str
                               JVM_NO_CHECK_AT_BOTTOM);
    Thread::set_current_pending_exception(&error);
  } else {
    allocate_and_throw(Symbols::java_lang_ClassNotFoundException(), &str
                       JVM_NO_CHECK_AT_BOTTOM);
  }
}

void Throw::array_store_exception(ErrorMsgTag err JVM_TRAPS) {
  allocate_and_throw(Symbols::java_lang_ArrayStoreException(), err
                     JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::null_pointer_exception(ErrorMsgTag err JVM_TRAPS) {
  allocate_and_throw(Symbols::java_lang_NullPointerException(), err
                     JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::array_index_out_of_bounds_exception(ErrorMsgTag err JVM_TRAPS) {
  allocate_and_throw(Symbols::java_lang_ArrayIndexOutOfBoundsException(), err
                     JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::illegal_access(FailureMode fail_mode JVM_TRAPS) {
  if (fail_mode == ErrorOnFailure) {
    error(illegal_access_error JVM_NO_CHECK_AT_BOTTOM);
  } else {
    allocate_and_throw(Symbols::java_lang_IllegalAccessException(),
                       illegal_access_exception JVM_NO_CHECK_AT_BOTTOM);
  }
}

void Throw::illegal_monitor_state_exception(ErrorMsgTag err JVM_TRAPS) {
  allocate_and_throw(Symbols::java_lang_IllegalMonitorStateException(),
                     err JVM_NO_CHECK_AT_BOTTOM);
}

#if ENABLE_CLDC_11
void Throw::interrupted_exception(ErrorMsgTag err JVM_TRAPS) {
  allocate_and_throw(Symbols::java_lang_InterruptedException(),
                     err JVM_NO_CHECK_AT_BOTTOM);
}
#endif

#if ENABLE_ISOLATES
void Throw::isolate_resource_error(ErrorMsgTag err JVM_TRAPS) {
  allocate_and_throw(Symbols::com_sun_cldc_isolate_IsolateResourceError(),
                     err JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::isolate_startup_exception(ErrorMsgTag err JVM_TRAPS) {
  allocate_and_throw(Symbols::com_sun_cldc_isolate_IsolateStartupException(),
                     err JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::isolate_state_exception(ErrorMsgTag err JVM_TRAPS) {
  allocate_and_throw(Symbols::com_sun_cldc_isolate_llegalIsolateStateException(),
                     err JVM_NO_CHECK_AT_BOTTOM);
}
#endif 

void Throw::illegal_thread_state_exception(ErrorMsgTag err JVM_TRAPS) {
  allocate_and_throw(Symbols::java_lang_IllegalThreadStateException(),
                     err JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::incompatible_class_change_error(ErrorMsgTag err JVM_TRAPS) {
  if (err == empty_message) {
    err = class_changed;
  }
  allocate_and_throw(Symbols::java_lang_IncompatibleClassChangeError(),
                     (char*)"IncompatibleClassChangeError",
                     err JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::instantiation(FailureMode fail_mode JVM_TRAPS) {
  if (fail_mode == ErrorOnFailure) {
    error(instantiation_error JVM_NO_CHECK_AT_BOTTOM);
  } else {
    allocate_and_throw(Symbols::java_lang_InstantiationException(),
                       instantiation_exception JVM_NO_CHECK_AT_BOTTOM);
  }
}

void Throw::arithmetic_exception(ErrorMsgTag err JVM_TRAPS) {
  allocate_and_throw(Symbols::java_lang_ArithmeticException(), err
                     JVM_NO_CHECK_AT_BOTTOM);
}

void Throw::unsatisfied_link_error(Method* method JVM_TRAPS) {
  UsingFastOops fast_oops;
  Symbol::Fast method_name_symbol = method->name();
  String::Fast method_name_string =
      Universe::new_string(&method_name_symbol JVM_CHECK);
  allocate_and_throw(Symbols::java_lang_UnsatisfiedLinkError(),
                     (char*)"UnsatisfiedLinkError", &method_name_string
                     JVM_NO_CHECK_AT_BOTTOM);
}

#if !ROMIZED_PRODUCT
void Throw::initialize(JVM_SINGLE_ARG_TRAPS) {
  UsingFastOops fast_oops;
  InstanceClass::Fast exception_class =
     SystemDictionary::resolve(Symbols::java_lang_OutOfMemoryError(),
                               ErrorOnFailure JVM_CHECK);
  *Universe::out_of_memory_error_instance() =
      Universe::new_instance(&exception_class JVM_NO_CHECK_AT_BOTTOM);
}
#endif

void Throw::uncatchable(JVM_SINGLE_ARG_TRAPS) {
  JVM_IGNORE_TRAPS;
  Thread::set_current_pending_exception(Universe::string_class());
}

ReturnOop Throw::add_message_prefix(char *prefix, String *message JVM_TRAPS) {
#if !ENABLE_STACK_TRACE
  return message->obj();
#else
  int prefixlen = jvm_strlen(prefix);
  int len = prefixlen;
  int msglen = 0;
  if (message->not_null()) {
    msglen = message->length();
    len += 2 + msglen;
  }
  UsingFastOops fast_oops;
  TypeArray::Fast char_array = Universe::new_char_array(len JVM_NO_CHECK);
  if (char_array.is_null()) {
    Thread::clear_current_pending_exception();
    return message->obj();
  }
  jchar *ptr = (jchar*)char_array().base_address();
  for (int i=0; i<prefixlen; i++) {
    *ptr ++ = (jchar)(*prefix++);
  }
  if (message->not_null()) {
    *ptr ++ = (jchar)':';
    *ptr ++ = (jchar)' ';
    TypeArray::Raw v = message->value();
    jchar *msgptr = ((jchar*)v().base_address()) + message->offset();
    for (int i=0; i<msglen; i++) {
      *ptr ++ = *msgptr++;
    }
  }
  GUARANTEE(ptr == (jchar*)char_array().base_address() + len, "sanity");
  String::Fast str = Universe::new_string(&char_array, 0, len JVM_NO_CHECK);
  if (str.is_null()) {
    Thread::clear_current_pending_exception();
    return message->obj();
  } else {
    return str.obj();
  }
#endif
}
