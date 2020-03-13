#! /bin/bash
if [ -e Makefile ]
then
	make distclean -s
fi
rm -fR GeneratedFiles release debug html latex *.qm refman.pdf README.md
