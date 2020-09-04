#!/bin/bash

if ! type node-gyp
then
	echo "You need to install node-gyp to compile this shit"
	
	if ! type npm
	then
		echo "Please, use your package manager of choice to install npm (node package manager)."
		exit 1
	fi
	
	read -p 'Install now? [Y/n] ' ans; [ "$ans" == "n" ] && exit 1
	
	npm install -g node-gyp
fi

if ! [ -d build ]
then
	mkdir -p build
fi

node-gyp configure rebuild
