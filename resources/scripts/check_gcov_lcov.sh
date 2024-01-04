#!/bin/bash

COMMAND=""
OS_TYPE=""
tool_check="check"
tool_lcov="lcov"
tool_gcov="gcov"

# Определение типа операционной системы
case "$(uname)" in
  "Darwin")
    echo "ОС: MacOS"
    OS_TYPE="MacOS"
    COMMAND="brew install"
    ;;
  "Linux")
    echo "ОС: Linux"
    OS_TYPE="Linux"
    COMMAND="sudo apt-get install -y"  # -y для автоматического подтверждения установки без запросов
    ;;
  *)
    echo "Операционная система не поддерживается."
    exit 1
    ;;
esac

# Проверяем check на MacOS
if [ "$OS_TYPE" == "MacOS" ]; then
  if pkg-config --modversion check &> /dev/null; then
    echo "Утилита $tool_check уже установлена."
  else
    echo "Утилита $tool_check не установлена."
    read -p "Хотите установить утилиту $tool_check? [y/n] " response
    if [ "$response" == "y" ]; then
      $COMMAND $tool_check
      echo "Утилита $tool_check установлена."
    else
      echo "Утилита $tool_check не будет установлена."
    fi
  fi
fi

# Проверяем check на Linux
if [ "$OS_TYPE" == "Linux" ]; then
  if dpkg -s check &> /dev/null; then
    echo "Утилита $tool_check уже установлена."
  else
    echo "Утилита $tool_check не установлена."
    read -p "Хотите установить утилиту $tool_check? [y/n] " response
    if [ "$response" == "y" ]; then
      sudo apt-get update
      $COMMAND $tool_check
      echo "Утилита $tool_check установлена."
    else
      echo "Утилита $tool_check не будет установлена."
    fi
  fi
fi

# Проверяем lcov
if ! command -v $tool_lcov &> /dev/null; then
  echo "Утилита $tool_lcov не установлена."
  read -p "Хотите установить утилиту $tool_lcov? [y/n] " response
  if [ "$response" == "y" ]; then
    # Команды для установки утилиты
    if [ "$OS_TYPE" == "Linux" ]; then
      sudo apt-get update
    fi
      $COMMAND $tool_lcov
      echo "Утилита $tool_lcov установлена."
  else
    echo "Утилита $tool_lcov не будет установлена."
  fi
else
  echo "Утилита $tool_lcov уже установлена."
fi

# Проверяем gcov
if ! command -v $tool_gcov &> /dev/null; then
  echo "Утилита $tool_gcov не установлена."
  read -p "Хотите установить утилиту $tool_gcov? [y/n] " response
  if [ "$response" == "y" ]; then
    # Команды для установки утилиты
    if [ "$OS_TYPE" == "Linux" ]; then
      sudo apt-get update
    fi
	if [ "$OS_TYPE" == "MacOS" ]; then
      xcode-select --install
    fi
	  $COMMAND $tool_gcov
      echo "Утилита $tool_gcov установлена."
  else
    echo "Утилита $tool_gcov не будет установлена."
  fi
else
  echo "Утилита $tool_gcov уже установлена."
fi

# Проверяем cppcheck
if ! command -v cppcheck &> /dev/null; then
  echo "Утилита cppcheck не установлена."
  read -p "Хотите установить утилиту cppcheck? [y/n] " response
  if [ "$response" == "y" ]; then
    if [ "$OS_TYPE" == "Linux" ]; then
      sudo apt-get update
      sudo apt-get install -y cppcheck
      echo "Утилита cppcheck установлена."
    fi
    if [ "$OS_TYPE" == "MacOS" ]; then
      brew install cppcheck
      echo "Утилита cppcheck установлена."
    fi
  else
    echo "Утилита cppcheck не будет установлена."
  fi
else
  echo "Утилита cppcheck уже установлена."
fi