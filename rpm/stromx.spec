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

Name:           stromx
Version:
Release:
Summary:        Library for near real-time, parallel processing of data
Group:          Development/Libraries/C and C++

License:        Apache 2.0
URL:            http://www.stromx.org
Source0:        %{name}-%{version}.tar.bz2
# BuildArch:      noarch

%if 0%{?suse_version}
BuildRequires:  cmake gcc-c++ libzip-devel opencv-devel libxerces-c-devel python-devel boost-devel doxygen cppunit-devel
%else
BuildRequires:  cmake gcc-c++ libzip-devel opencv-devel xerces-c-devel python-devel boost-devel doxygen lapack cppunit-devel
%endif
# Requires:       

%description
This package contains the %{name} library for near real-time, parallel processing of data.


%package        devel
Summary:        Development files for %{name}
Group:          Development/Libraries/C and C++
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.


%package        python
Summary:        Python bindings for %{name}
Group:          Development/Libraries/Python
%if 0%{?suse_version}
%py_requires
%endif
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    python
The %{name}-python package contains Python bindings for %{name}.

%package        doc


Summary:        Documentation and examples for %{name}
Group:          Documentation/Other

%description    doc
This package contains the documentation and tutorials for the %{name} library.

%prep
%setup -q


%build
%{__mkdir} build
cd build
cmake -DCMAKE_SKIP_RPATH=ON \
      -DCMAKE_INSTALL_PREFIX='%{_prefix}' \
      -DCMAKE_BUILD_TYPE=Release \
%if "%{?_lib}" == "lib64"
      -DLIB_SUFFIX=64 \
%endif
      -DPYTHON_INSTALL_DIR=%{python_sitearch} \
      -DDOC_INSTALL_DIR=%{_docdir} \
      ..

make %{?_smp_mflags}
make %{?_smp_mflags} doc


%install
cd build
make install DESTDIR=%{buildroot}
make test ARGS="-V"
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

%files devel
%defattr(0644, root, root, 0755)
%doc
%{_includedir}/*
%{_libdir}/*.so
%{_datadir}/stromx

%files python
%defattr(0644, root, root, 0755)
%{python_sitearch}/stromx

%files doc
%defattr(-, root, root, 0755)
%{_docdir}/stromx

%changelog
