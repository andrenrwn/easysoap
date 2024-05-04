%define ver 0.6.2
%define rel 1
%define prefix /usr

Summary: SOAP Implemented in C++
Name: EasySoap++-0.6.2
Version: %{version}
Release: %{release}
#Source: http://prdownloads.sourceforge.net/easysoap/EasySoap++-%{ver}.tar.gz
License: LGPL
URL: http://easysoap.sourceforge.net
Group: System Environment/Libraries
Packager: Blaise St-Laurent <blaise@geeky.net>
Provides: interface(devfs)
#Buildroot: /var/tmp/%{name}-root

%description
EasySoap++ is a lightweight SOAP implementation written in C++. It implements
most of the SOAP specification, and interoperates well with many other 
soap implementations.

%package devel
Summary: Libraries, includes and docs for development with EasySoap++
Requires: %{name} = %{ver}
Group: Development/Libraries
Provides: interface(devfs)

%description devel
Include files for easy EasySoap++0.6.2

%prep
rm -rf $RPM_BUILD_ROOT
%setup -D -T

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{prefix} \
--with-expat=%{_topdir}/devfs/opt/expat250 \
--with-openssl=%{_topdir}/devfs/usr
make

%install
make prefix=${RPM_BUILD_ROOT}%{prefix} TOPDIR=%{_topdir} install

%post
ldconfig

%postun
ldconfig

%clean
rm -rf ${RPM_BUILD_ROOT}

%files
%defattr(-,root,root)
%doc AUTHORS ChangeLog COPYING
%{_libdir}/libeasysoap.so*
%{_libdir}/libeasysoap-0.6.2.so

%files devel
%defattr(-,root,root)
%{_libdir}/libeasysoap.a
%{_libdir}/libeasysoap.la
%{_includedir}/easysoap/*.h
