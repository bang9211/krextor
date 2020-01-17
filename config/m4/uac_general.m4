
dnl ============================================================================
dnl Get OS name, version & machine information 
dnl ============================================================================

AC_DEFUN([UAC_SYSTEM_INFO], [
	dnl get os information

	ac_sys_system=`uname -s`
	ac_sys_release=`uname -r`

	if test "$ac_sys_system" = "AIX" -o "$ac_sys_system" = "Monterey64" -o \
		"$ac_sys_system" = "UnixWare"; then
		ac_sys_release=`uname -v`
	else
		if test "$ac_sys_system" = "CYGWIN"; then
			ac_sys_system=`uname -o`
			ac_sys_release=`uname -r | awk -F'(' '{print $1}'`
		fi
	fi

	AC_MSG_CHECKING(ua_os_name)
	AC_SUBST(ua_os_name)
	ua_os_name=`echo $ac_sys_system | tr '[/ -.]' '_' | tr -d '-' | tr '[[A-Z]]' '[[a-z]]'`
	AC_MSG_RESULT($ua_os_name)

	AC_MSG_CHECKING(ua_os_version)
	AC_SUBST(ua_os_version)
	ua_os_version=`echo $ac_sys_release | tr '/' '_' | tr ' ' '_' | tr '-' '_' | tr '.' '_' | tr '[[A-Z]]' '[[a-z]]'`
	AC_MSG_RESULT($ua_os_version)

	AC_MSG_CHECKING(ua_os_info)
	AC_SUBST(ua_os_info)
	ua_os_info="$ua_os_name$ua_os_version"
	case $ua_os_info in
		'')	ua_os_info="unknown";;
	esac
	AC_MSG_RESULT($ua_os_info)

	AC_MSG_CHECKING(ua_mach_name)
	AC_SUBST(ua_mach_name)
	ac_mach_name=`uname -m`
	ua_mach_name=`echo $ac_mach_name | tr '/' '_' | tr ' ' '_' | tr '-' '_' | tr '.' '_' | tr '[[A-Z]]' '[[a-z]]'`
	AC_MSG_RESULT($ua_mach_name)

	AC_MSG_CHECKING(ua_cpu_name)
	AC_SUBST(ua_cpu_name)
	if test "$ac_sys_system" = "HP-UX"; then
		ac_cpu_name=`uname -m`
	else
		ac_cpu_name=`uname -p`
	fi
	ua_cpu_name=`echo $ac_cpu_name | tr '/' '_' | tr ' ' '_' | tr '-' '_' | tr '.' '_' | tr '[[A-Z]]' '[[a-z]]'`
	AC_MSG_RESULT($ua_cpu_name)

])

dnl ============================================================================
dnl Get compile information 
dnl ============================================================================

AC_DEFUN([UAC_COMPILE_INFO], [
	dnl get compiler information

	AC_REQUIRE([UAC_SYSTEM_INFO])
	AC_PROG_CC(cc gcc)
	AC_PROG_CXX(cxx aCC CC g++ gcc c++)

	AC_SUBST(CFLAGS_OPTIMIZE)
	AC_SUBST(CFLAGS_WARN_ALL)
	AC_SUBST(CINCLUDE)
	AC_SUBST(CXXINCLUDE)
	AC_SUBST(DEPEND_COMMAND)
	AC_SUBST(MAKDEDEPEND)
	AC_SUBST(SHLDFLAGS)
	AC_SUBST(SH_NEEDLIBS_BEGIN)
	AC_SUBST(SH_NEEDLIBS_EACH)
	AC_SUBST(SH_NEEDLIBS_END)
	AC_SUBST(SUFSHARED)
	AC_SUBST(LINKER)
	AC_SUBST(SHLINKER)

	CFLAGS=`echo $CFLAGS | sed s/-g// | sed s/-O2//`
	CXXFLAGS=`echo $CXXFLAGS | sed s/-g// | sed s/-O2//`
	SHLDFLAGS=
	SH_NEEDLIBS_BEGIN=""
	SH_NEEDLIBS_EACH=""
	SH_NEEDLIBS_END=""
	SUFSHARED=".so"
	LINKER="$CC"
	SHLINKER="$CC"
	MAKEDEPEND="$CC"

	AC_MSG_CHECKING(ua_compile_64bit)
	AC_SUBST(ua_compile_64bit)
	ua_compile_64bit="yes"
	AC_ARG_ENABLE(64bit, 
		AC_HELP_STRING([--disable-64bit],[compile to 32bit binary (default 64bit=yes)]),
		[ua_compile_64bit="no"], [ua_compile_64bit="yes"] )
	AC_MSG_RESULT($ua_compile_64bit)

dnl	if test "$CC" = "gcc" -o "$ac_cv_prog_cxx_g" = "yes"; then 
	if test "$CC" = "gcc"; then 
		case $ua_compile_64bit in
			"yes") 
				CFLAGS=`echo $CFLAGS | sed s/-m64//`
				CXXFLAGS=`echo $CXXFLAGS | sed s/-m64//`
				LDFLAGS=`echo $LDFLAGS | sed s/-m64//`
				SHLDFLAGS=`echo $SHLDFLAGS | sed s/-m64//`
				CFLAGS="$CFLAGS -m64"
				CXXFLAGS="$CXXFLAGS -m64"
				LDFLAGS="$LDFLAGS -m64"
				SHLDFLAGS="$SHLDFLAGS -m64"
			;;
		esac;
	fi

	case $ua_os_name in
		*osf*) 
dnl			if test "$CC" = "gcc" -o "$ac_cv_prog_cxx_g" = "yes";
			if test "$CC" = "gcc";
			then 
				CPPFLAGS="-D__OSF_ -D_REENTRANT"
				CFLAGS_OPTIMIZE="-O -fomit-frame-pointer"
				CFLAGS_WARN_ALL="-Wall"
			else
				#DEC Compiler automatically uses reentrant version by thread specific data
				#C_USE_TEMPLATE_REPOSITORY="true"
				CPPFLAGS="-D__OSF_ -D_REENTRANT -D__USE_STD_IOSTREAM"
				CFLAGS="-pthread -fnousing_std -fstdnew $CFLAGS"
				CXXFLAGS="-pthread -fnousing_std -fstdnew $CXXFLAGS"
				LDFLAGS="-pthread $LDFLAGS"
				#LIB_CFLAGS="-ptr \$(TEMPLATE_REPOSITORY_DIR)"
				BIN_LDFLAGS="-ptr ./cxx_repository -ptr \$(TEMPLATE_REPOSITORY_DIR)"
			fi
		;;
		*nonstop*)
			# -D_POSIX_PTHREAD_SEMANTICS for sigwait()
			SHLINKER="$CC -G -Wl,-all"
			CPPFLAGS="-D_REENTRANT"
			CFLAGS="-oldcpp -tp -B99 $CFLAGS"
			CXXFLAGS="-oldcpp -tp -B99 $CXXFLAGS"
			CFLAGS_OPTIMIZE="-O"
			CFLAGS_WARN_ALL=""
		;;	
		*sun*)
dnl			if test "$CC" = "gcc" -o "$ac_cv_prog_cxx_g" = "yes";
			if test "$CC" = "gcc";
			then 
				CPPFLAGS="-D__SUN__ -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS -DPIC $CPPFLAGS"
				CFLAGS="-Wshadow -fPIC $CFLAGS"
				CXXFLAGS="-Wshadow -fPIC $CXXFLAGS"
				CFLAGS_OPTIMIZE="-O -fomit-frame-pointer"
				CFLAGS_WARN_ALL="-Wall"
				SHLDFLAGS="$SHLDFLAGS -G -z allextract -Wl,-z,muldefs"
dnl				SHLDFLAGS="-shared"
dnl				SH_NEEDLIBS_BEGIN="-Wl,--whole-archive"
dnl				SH_NEEDLIBS_END="-Wl,--no-whole-archive"
			else
				# -D_POSIX_PTHREAD_SEMANTICS for sigwait()
				CPPFLAGS="-D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS $CPPFLAGS"
			fi
			#LIBS="-lpthread"
		;;
		*hpux*)
			SHLINKER="ld"
			SUFSHARED=".sl"
			CPPFLAGS="$CPPFLAGS -D__HPUX__ -D_REENTRANT"
			CINCLUDE="-I/usr/local/include"

dnl			if test "$CC" = "gcc" -o "$ac_cv_prog_cxx_g" = "yes";
			if test "$CC" = "gcc";
			then
				CPPFLAGS="$CPPFLAGS -D_POSIX_PTHREAD_SEMANTICS -DPIC"
				CFLAGS_OPTIMIZE="-O -fomit-frame-pointer"
				CFLAGS_WARN_ALL="-Wall"
			else
				if test "$ua_mach_name" = "ia64";
				then
					#   warning code for ITANIUM HPUX cc compiler:
					#   +w      warning all
					#   +W431   No uses of 'notUsed' were detected
					#   +W392   converion unncecessary; expression was already of type
					#   +W818   'long' is larger than type 'int', truncation in value may result.
					#   +W887   (64)Type 'int' is smaller than type 'unsigned long'
					FLAGS_WARN_ALL="+w +W392 +W431 +W818"

					CPPFLAGS="$CPPFLAGS -D_RWSTD_MULTI_THREAD"
					if test "$ua_compile_64bit" = "yes";
					then
						#ITANIUM-64BIT
						CFLAGS="-Ae +DD64 $CFLAGS"  # +Z is default
						CXXFLAGS="-Ae +DD64 $CXXFLAGS"  # +Z is default
						LDFLAGS="+DD64 $LDFLAGS"
						CFLAGS_WARN_ALL="$CFLAGS_WARN_ALL +W818 +W887"
					else
						#ITANIUM-32BIT
						CFLAGS="-Ae +DD32 +z $CFLAGS"
						CXXFLAGS="-Ae +DD32 +z $CXXFLAGS"
						LDFLAGS="+DD32 $LDFLAGS"
					fi
					SHLDFLAGS="-dynamic -b +forceload"
				else
					#   warning code for PA-RISC HPUX c compiler:
					#   +w1     warning all
					#   +W474   By default 'const' qualified objects ...
					#   +W732   Different types treated as unsigned for ==
					#   +W604   Pointers are not assignment-compatible.
					#   +W530   Casting from loose to strict alignment
					#   +W720   Assignment may overflow integer     (for 64Bit)
					#   +W728   Cast converts long* to int*.        (for 64Bit)
					#   +W509   Condition always evaluates to true
					#   +W510   Condition always evaluates to true
					#   +W849   ?

					if test "$ua_os_version" = "B1100"
					then
						CFLAGS_WARN_ALL="+w1 +W 474 +W 732 +W 604 +W 530 +W 509 +W 510 +W 849 "
					else
						CFLAGS_WARN_ALL="+w1 +W474 +W732 +W604 +W530 +W509 +W510 +W849"
					fi

					CPPFLAGS="$CPPFLAGS -D_RWSTD_MULTI_THREAD"
					if test "$ua_compile_64bit" = "yes";
					then
						#PARISC-64bit
						CFLAGS="-Ae +DA2.0W $CFLAGS" 
						CXXFLAGS="-AA +DA2.0W $CXXFLAGS" 
						LDFLAGS="+DA2.0W $LDFLAGS"
						SHLDFLAGS="-dynamic -b +forceload"
						CFLAGS_WARN_ALL="$CFLAGS_WARN_ALL +W720 +W728"
					else
						#PARISC-32bit
						CFLAGS="-Ae +DAportable +z $CFLAGS" 
						CXXFLAGS="-AA +DAportable +z $CXXFLAGS" 
						LDFLAGS="+DAportable -Wl,+s $LDFLAGS"
						SHLDFLAGS="-dynamic -b +vallcompatwarnings +s"
							#+s enables user to use SHLIB_PATH it is default in ITANIUM cc
						SH_NEEDLIBS_EACH="-Fl"
					fi
				fi
				CFLAGS_OPTIMIZE="-O"
				#LIBS="-lpthread"
			fi
		;;
		*linux*)
			CPPFLAGS="-D__LINUX__ -DPIC -D_GNU_SOURCE -D_REENTRANT"
			CFLAGS="-Wshadow -fPIC $CFLAGS"
			CXXFLAGS="-Wshadow -fPIC $CXXFLAGS"
			CFLAGS_OPTIMIZE="-O -fomit-frame-pointer"
			CFLAGS_WARN_ALL="-Wall"
			SHLDFLAGS="-shared"
			SH_NEEDLIBS_BEGIN="-Wl,--whole-archive"
			SH_NEEDLIBS_END="-Wl,--no-whole-archive"
		;;
		*)
		;;
	esac

	AC_ARG_ENABLE(debug, 
		AC_HELP_STRING([--disable-debug],[turn off debug option -g (default debug=yes)]),
		[CFLAGS="$CFLAGS" CXXFLAGS="$CXXFLAGS"], [CFLAGS="$CFLAGS -g" CXXFLAGS="$CXXFLAGS -g"] )

	AC_ARG_ENABLE(optimize, 
		AC_HELP_STRING([--disable-optimize],[turn off optimize option -o (default optimze=yes)]),
		[CFLAGS="$CFLAGS" CXXFLAGS="$CXXFLAGS"], [CFLAGS="$CFLAGS $CFLAGS_OPTIMIZE" CXXFLAGS="$CXXFLAGS $CFLAGS_OPTIMIZE"] )

	AC_ARG_ENABLE(warning, 
		AC_HELP_STRING([--disable-warning],[turn off notifying compile warning (default warning=yes)]),
		[CFLAGS="$CFLAGS" CXXFLAGS="$CXXFLAGS"], [CFLAGS="$CFLAGS $CFLAGS_WARN_ALL" CXXFLAGS="$CXXFLAGS $CFLAGS_WARN_ALL"] )

	AC_ARG_ENABLE(xopen,
		AC_HELP_STRING([--enable-xopen],[turn on optimize option -DXOPEN_SOURCE_EXTENDED (default xopen=no)]),
		[CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE_EXTENDED" LIBS="$LIBS -lxnet"], [CPPFLAGS="$CPPFLAGS"] )

	AC_ARG_ENABLE(uasys_home, 
		AC_HELP_STRING([--enable-uasys_home],[do install binary to UASYS_HOME]),
		[ac_copy_uasys_home="yes"], [ac_copy_uasys_home="no"] )
	AC_MSG_CHECKING(ua_copy_uasys_home)
	AC_SUBST(ua_copy_uasys_home)
	ua_copy_uasys_home=$ac_copy_uasys_home
	AC_MSG_RESULT($ua_copy_uasys_home)

	case $MAKEDEPEND in
		*cpp*)
			DEPEND_COMMAND="$MAKEDEPEND -MM \$(CPPFLAGS) \$(CINCLUDE) \$(CXXINCLUDE)"
		;;
		*gcc*)
			DEPEND_COMMAND="$MAKEDEPEND -M \$(CPPFLAGS) \$(CINCLUDE) \$(CXXINCLUDE)"
		;;
		*)
			DEPEND_COMMAND="$MAKEDEPEND -f \$DEPEND_FILE -- \$(CPPFLAGS) \$(CINCLUDE) \$(CXXINCLUDE) -- \$DEPEND_SOURCES 2> null"
		;;
	esac
])


dnl ======================================================================
dnl Check for sockaddr_in6
dnl ======================================================================
AC_DEFUN([UAC_STRUCT_SIN6], [
	AC_CACHE_CHECK([for sockaddr_in6], ac_cv_sin6,
		[AC_EGREP_HEADER(struct sockaddr_in6, netinet/in.h, ac_cv_sin6=yes, ac_cv_sin6=no)])
	if test $ac_cv_sin6 = yes ;then
		AC_DEFINE([UA_HAVE_SIN6], 1, [Define to 1 if you have IPv6 structures and constants])
	fi
])

AC_DEFUN([UAC_SYS_SA_LEN], [
	AC_CACHE_CHECK([for sa_len], ac_cv_sa_len,
		[AC_TRY_COMPILE([#include <sys/types.h>
			#include <sys/socket.h>],
			[struct sockaddr t;t.sa_len = 0;], ac_cv_sa_len=yes,ac_cv_sa_len=no)])
	if test "$ac_cv_sa_len" = yes ;then
		AC_DEFINE([UA_HAVE_SA_LEN], 1, [Define to 1 if you have sa_len in struct sockaddr])
	fi
])

AC_DEFUN([UAC_LUA_INFO], [
	dnl get lua information

	ac_lua_h=false
	AC_CHECK_HEADER($LUA_HOME/include/lua.h, [ac_lua_h=true])
	ac_luajit_include_path="include"

	ac_luajit_luajit_h=false
	AC_CHECK_HEADER($LUA_HOME/include/luajit/luajit.h, [ac_luajit_luajit_h=true])
	ac_luajit_include_path="include/luajit"

	if test "$ac_luajit_luajit_h" = false ; then
		AC_CHECK_HEADER($LUA_HOME/include/luajit-2.0/luajit.h, [ac_luajit_luajit_h=true])
		ac_luajit_include_path="include/luajit-2.0"
	fi

	if test "$ac_luajit_luajit_h" = false ; then
		ac_use_luajit="no"
	else
		ac_use_luajit="yes"
		AC_ARG_ENABLE(luajit, 
			AC_HELP_STRING([--disable-luajit],[do not compile to use luajit]),
			[ac_use_luajit="no"], [ac_use_luajit="yes"] )
	fi

	AC_MSG_CHECKING(ua_has_lua)
	AC_SUBST(ua_has_lua)
	if test "$ac_lua_h" = true ; then
		ua_has_lua="yes"
	else
		if test "$ac_luajit_luajit_h" = true ; then
			ua_has_lua="yes"
		else
			ua_has_lua="no"
		fi
	fi
	AC_MSG_RESULT($ua_has_lua)

	AC_MSG_CHECKING(ua_use_luajit)
	AC_SUBST(ua_use_luajit)
	ua_use_luajit=$ac_use_luajit
	AC_MSG_RESULT($ua_use_luajit)

	AC_SUBST(ua_lua_include_path)
		if test  "$ua_use_luajit" = "yes" ; then
			ua_lua_include_path="$ac_luajit_include_path"
		else
			ua_lua_include_path="$ac_lua_include_path"
		fi
	AC_MSG_CHECKING(ua_lua_include_path)
	AC_MSG_RESULT($ua_lua_include_path)

	AC_SUBST(ua_lua_libs)
	ua_lua_libs="liblua"
	if test -z "$LUA_HOME"; then
		ua_lua_libs=""
	else
		ua_lua_libs="$LUA_HOME/lib/liblua"
		if $ac_luajit_luajit_h ; then 
			if test "$ac_use_luajit" = "yes" ; then 
				ua_lua_libs=""
			fi	
		fi 
	fi

	AC_MSG_CHECKING(ua_lua_libs)
	AC_MSG_RESULT($ua_lua_libs)
])
