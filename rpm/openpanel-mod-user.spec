%define version 0.9.6

%define libpath /usr/lib
%ifarch x86_64
  %define libpath /usr/lib64
%endif

Summary: User management module
Name: openpanel-mod-user
Version: %version
Release: 1
License: GPLv2
Group: Development
Source: http://packages.openpanel.com/archive/openpanel-mod-user-%{version}.tar.gz
Patch1: openpanel-mod-user-00-makefile
BuildRoot: /var/tmp/%{name}-buildroot
Requires: openpanel-core >= 0.8.3

%description
User management module
Openpanel user management module

%prep
%setup -q -n openpanel-mod-user-%version
%patch1 -p0 -b .buildroot

%build
BUILD_ROOT=$RPM_BUILD_ROOT
./configure
make

%install
BUILD_ROOT=$RPM_BUILD_ROOT
rm -rf ${BUILD_ROOT}
mkdir -p ${BUILD_ROOT}/var/opencore/modules/User.module
cp -rf ./usermodule.app ${BUILD_ROOT}/var/opencore/modules/User.module/
ln -sf usermodule.app/exec ${BUILD_ROOT}/var/opencore/modules/User.module/action
cp module.xml ${BUILD_ROOT}/var/opencore/modules/User.module/module.xml
cp *.png ${BUILD_ROOT}/var/opencore/modules/User.module/
install -m 755 verify ${BUILD_ROOT}/var/opencore/modules/User.module/verify

%post
mkdir -p /var/opencore/conf/staging/User
chown opencore:authd /var/opencore/conf/staging/User

%files
%defattr(-,root,root)
/
