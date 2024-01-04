#!/bin/bash

if [[ "$(uname)" == "Darwin" ]]; then

    if ! command -v brew &> /dev/null; then

    	echo "Homebrew не обнаружен. Установка..."
        /bin/bash -c "$(curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh)"

    else
        echo "Homebrew уже установлен."
    fi

else
    echo "Другая операционная система, не требуется установка Homebrew."
fi
