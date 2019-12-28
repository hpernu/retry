
# Automatically set the versions and release, circumvent problems with missing git command
%define version %((git tag || echo 0) | tail -n 1)
%define rel %(git rev-list %version.. --count || echo 0)
%define snapshot %(git rev-parse --short HEAD || echo 0)
%global debug_package %{nil}

Name:           retry
Version:        %version
Release:        %rel.%snapshot%{?dist}
Summary:        Retry a command until it succeeds
License:        MIT
Group:          Utilities
URL:            https://github.com/hpernu/retry
Source0:        %{name}.tar.gz
BuildRequires:	gcc
BuildRequires:  /usr/bin/git
BuildRequires:  make
BuildRequires:  /usr/bin/perl
BuildRequires:  /usr/bin/rpmbuild

%description
A tool to retry a program multiple times until it succeed(or a set amount of max tries)
and only output the output of a succesful run.
Intended for integrations where the other end might not be constantly available.

%prep
%setup -q -c

%build
%set_build_flags
make %{?_smp_mflags} CFLAGS="$CFLAGS" LDFLAGS="$LDFLAGS"

%install
%make_install

%check
make %{?_smp_mflags} test

%files
%defattr(0775,root,root,0775)
%{_bindir}/retry

%changelog
* Sat Dec 28 2019 Heikki Pernu 1.0
- First RPM version
