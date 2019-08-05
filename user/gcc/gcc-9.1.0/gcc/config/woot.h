#undef TARGET_WOOT
#define TARGET_WOOT 1

#undef LIB_SPEC
#define LIB_SPEC "-lc"

#undef STANDARD_STARTFILE_PREFIX
#define STANDARD_STARTFILE_PREFIX "/lib/"

#undef STARTFILE_SPEC
#define STARTFILE_SPEC "crt1.o%s crti.o%s crtbegin.o%s"

#undef ENDFILE_SPEC
#define ENDFILE_SPEC "crtend.o%s crtn.o%s"

#undef LINK_SPEC
#define LINK_SPEC "%{shared:-shared} %{static:-static} %{!shared: %{!static: %{rdynamic:-export-dynamic}}} -z max-page-size=4096"

#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()      \
  do {                                \
    builtin_define ("__woot__");      \
    builtin_define ("__WOOT__");      \
    builtin_define ("__unix__");      \
    builtin_assert ("system=woot");   \
    builtin_assert ("system=unix");   \
    builtin_assert ("system=posix");  \
  } while(0);
