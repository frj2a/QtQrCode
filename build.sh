#! /bin/bash
PROGRAM=QRcode
if [ -e Makefile ]
then
	make distclean -s
fi
rm -fR GeneratedFiles release debug *.qm
QT_OTIMIZA="linux-g++"
MAQUINA=`uname -m`
if [ "$MAQUINA" = "x86_64" ]
then
	QT_OTIMIZA="linux-g++-64"
fi
echo -e - - -Compilando o programa '\033[1;33m'$PROGRAM'\033[0m' para a plataforma '\033[1;33m'$MAQUINA'\033[0m' ...

lrelease -qt=qt5 $PROGRAM 2> /dev/null

cat Doxyfile.linux | sed s/"PROJECT_NUMBER[\ ]*= "/"PROJECT_NUMBER         = "`qmake -qt=qt5 -r -spec $QT_OTIMIZA "CONFIG-=debug_and_release release debug" "CONFIG+=release" 2>&1 | grep "Project MESSAGE:" | cut -d\  -f5`/ > Doxyfile && /usr/bin/make qmake_all

make $MAKEOPTS -s && \
strip -s $PROGRAM

rm -fR GeneratedFiles debug *.qm .qmake.stash
if [ -e /usr/bin/doxygen ]
then
	doxygen Doxyfile
fi
if [ -d latex ]
then
	cd latex
	make
	mv refman.pdf ..
	cd ..
	rm -fR latex
fi
rm -f Doxyfile
if [ -e /usr/bin/cloc ]
then
	cloc *.cpp *.h
fi
echo -e - - -Finalizada a compilação do programa '\033[1;33m'$PROGRAM'\033[0m' para a plataforma '\033[1;33m'$MAQUINA'\033[0m' ...
