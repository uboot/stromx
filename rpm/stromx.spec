#
# spec file for package [spectemplate]
#
# Copyright (c) 2010 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

# norootforbuild

Name:           libstromx
Version:        0.8.0
Release:        0
Summary:        Library for near real-time, parallel processing of data
Group:          Development/Libraries/C and C++

License:        Apache License 2.0
URL:            http://www.stromx.org
Source0:        %{name}-%{version}.tar.bz2
# BuildArch:      noarch

BuildRequires:  cmake gcc-c++ libzip-devel opencv-devel libxerces-c-devel doxygen cppunit-devel
%if 0%{?sle_version} == 120300 && 0%{?is_opensuse}
BuildRequires: pkgconfig(python) boost-devel
%else
%if 0%{?suse_version} > 1500
BuildRequires:  pkgconfig(python3) libboost_headers1_66_0-devel libboost_chrono1_66_0-devel libboost_date_time1_66_0-devel libboost_filesystem1_66_0-devel libboost_locale1_66_0-devel libboost_python-py3-1_66_0-devel libboost_regex1_66_0-devel libboost_serialization1_66_0-devel libboost_system1_66_0-devel libboost_thread1_66_0-devel  libboost_timer1_66_0-devel
%endif
%endif
# Requires:       

%description
This package contains the stromx library for near real-time, parallel processing of data.


%package        -n stromx-devel
Summary:        Development files for stromx
Group:          Development/Libraries/C and C++
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    -n stromx-devel
This package contains libraries and header files for
developing applications that use stromx.


%package        -n python3-stromx
Summary:        Python 3 bindings for stromx
Group:          Development/Libraries/Python
Obsoletes:      python3-stromx

%description    -n python3-stromx
This package contains Python bindings for stromx.


%package        -n stromx-doc

Summary:        Documentation and examples for stromx
Group:          Documentation/Other

%description    -n stromx-doc
This package contains the documentation and tutorials for the stromx library.


%prep
%setup -q


%build
%{__mkdir} build
cd build
cmake -DCMAKE_INSTALL_PREFIX='%{_prefix}' \
      -DCMAKE_BUILD_TYPE=Release \
%if "%{?_lib}" == "lib64"
      -DLIB_SUFFIX=64 \
%endif
%if 0%{?suse_version} > 1500
      -DPYTHON_INSTALL_DIR=%{python3_sitearch} \
      -DDOC_INSTALL_DIR=%{_docdir} \
      -DPYTHON_EXECUTABLE=/usr/bin/python3 \
      ..
%endif

make %{?_smp_mflags}
make %{?_smp_mflags} doc


%check
cd build
make test ARGS="-V"


%install
cd build
make install DESTDIR=%{buildroot}
find %{buildroot} -name '*.la' -exec rm -f {} ';'


%clean
rm -rf %{buildroot}


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(0644, root, root, 0755)
%doc
%{_libdir}/*.so.*
%exclude %{_datadir}/stromx

%files -n stromx-devel
%defattr(0644, root, root, 0755)
%doc
%{_includedir}/*
%{_libdir}/*.so
%{_datadir}/stromx

%files -n python3-stromx
%defattr(0644, root, root, 0755)
%{python3_sitearch}/stromx

%files -n stromx-doc
%defattr(-, root, root, 0755)
%{_docdir}/stromx

%changelog
