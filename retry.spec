Summary: Retry tool
Name: retry
Version: 2
Release: 1
License: GPL
Group: Utilities
URL: https://github.com/hpernu/retry
Source: %{name}.tar.gz
BuildRequires: make
BuildRequires: gcc
BuildRequires: rpm-build

%description
Retry tool

%prep
rm -rf $RPM_BUILD_ROOT

%setup -q
 
%build
make %{_smp_mflags}

%install
%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(0775,root,root,0775)
%{_bindir}/retry

%changelog
* Wed Apr 25 2018 Heikki Pernu
- Created spec file
