#!/bin/bash -xe
command -v make >/dev/null || yum install -y make
command -v sudo >/dev/null || yum install -y sudo
command -v perl >/dev/null || yum install -y perl
