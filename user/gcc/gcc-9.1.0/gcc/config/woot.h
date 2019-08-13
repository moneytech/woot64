#undef TARGET_WOOT
#define TARGET_WOOT 1

#undef STANDARD_STARTFILE_PREFIX
#define STANDARD_STARTFILE_PREFIX "/lib/"

#undef LIB_SPEC
#define LIB_SPEC "-lc"

#undef STARTFILE_SPEC
#define STARTFILE_SPEC \
	" %{shared:; static:crt1.o%s; static-pie:rcrt1.o%s; PIE:Scrt1.o%s; :crt1.o%s} \
	crti.o%s \
	%{static:crtbeginT.o%s; shared|static-pie|PIE:crtbeginS.o%s; :crtbegin.o%s} "

#undef ENDFILE_SPEC
#define ENDFILE_SPEC \
	" %{static:crtend.o%s; shared|static-pie|PIE:crtendS.o%s; :crtend.o%s} \
	crtn.o%s "

#undef LINK_SPEC
#define LINK_SPEC " %{shared:-shared} %{static:-static} %{!shared: %{!static: %{rdynamic:-export-dynamic}}} -z max-page-size=4096 "

#if defined(HAVE_LD_EH_FRAME_HDR)
#define LINK_EH_SPEC " %{!static|static-pie:--eh-frame-hdr} "
#endif

#ifdef HAVE_LD_AS_NEEDED
#define USE_LD_AS_NEEDED 1
#endif

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
