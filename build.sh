#!/bin/bash

RED='\033[0;31m'	# red
GREEN='\033[0;32m'	# green
YELLOW='\e[33m'		# yellow
BLUE='\033[1;34m'	# blue
CYAN='\e[96m'		# cyan
BOLD='\e[1m'		# bold
NO_COLOR='\033[0m'	# no color

COMM_DIR=./Communication
CONV_DIR=./Conversion
VIEW_DIR=./Viewer
CONT_DIR=./Controller
EXEC_DIR=./Executables
QT_BUILD_DIR=./Build_Controller
QT_CONT_PRO_FILE=controller.pro

function cleaner() {
	make -s -C ${COMM_DIR} clean
	make -s -C ${CONV_DIR} clean
	make -s -C ${VIEW_DIR} clean
	rm -f ${CONT_DIR}/controller
	rm -rf $EXEC_DIR
	rm -rf $QT_BUILD_DIR
	rm -f CanTouchThis
	echo -e ${NO_COLOR}
}

function compile() {
	echo -e "${GREEN}Compiling ${BLUE}$1${GREEN} source files...${YELLOW}"
	make -C $2 || exit
	echo -e ${NO_COLOR}
}

function check_dir() {
	if [ -d $1 ]
	then
		rm -rf $1/*
	else
		mkdir $1
	fi
}

function prepare_exec() {
	check_dir ${EXEC_DIR}
	mv ${COMM_DIR}/Communication ${CONV_DIR}/Conversion ${VIEW_DIR}/Viewer \
		${EXEC_DIR}
	mv ${QT_BUILD_DIR}/controller ${EXEC_DIR}/CanTouchThis
}

function compile_controller() {
	echo -e "${GREEN}Compiling ${BLUE}Controller${GREEN} source files...${YELLOW}"
	check_dir ${QT_BUILD_DIR}
	cd ${QT_BUILD_DIR}
	qmake .${CONT_DIR}/$QT_CONT_PRO_FILE
	cd ..
	make -C ${QT_BUILD_DIR} || exit
	echo -e ${NO_COLOR}
}

echo -e "${BOLD}Welcome to Can Touch This GNU/Linux Build Version!${NO_COLOR}\n"

if [[ $1 && $1 == "clean" ]]
then
	echo -e "${CYAN}Cleaning the built data..."
	cleaner
else
	trap cleaner INT
	compile "Communication" "${COMM_DIR}"
	compile "Conversion" "${CONV_DIR}"
	compile "Viewer" "${VIEW_DIR}"
	compile_controller
	prepare_exec
	echo -e "${CYAN}${BOLD}Can Touch This has been installed successfully."
	echo -e "Executable file: ./Executable/CanTouchThis"
fi
echo -ne ${NO_COLOR}
