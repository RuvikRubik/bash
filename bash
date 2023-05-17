#!/bin/bash

# Funkcja dodająca grupę
add_group() {
    read -p "Podaj nazwę grupy: " group_name
    sudo groupadd $group_name
    echo "Grupa $group_name została utworzona."
}

# Funkcja dodająca użytkownika
add_user() {
    read -p "Podaj nazwę użytkownika: " username
    sudo useradd $username
    echo "Użytkownik $username został utworzony."
}

# Funkcja przypisująca użytkownika do grupy
add_user_to_group() {
    read -p "Podaj nazwę użytkownika: " username
    read -p "Podaj nazwę grupy: " group_name
    sudo usermod -a -G $group_name $username
    echo "Użytkownik $username został przypisany do grupy $group_name."
}

# Główna pętla menu
while true; do
    echo "1. Dodaj grupę"
    echo "2. Dodaj użytkownika"
    echo "3. Dodaj użytkownika do grupy"
    echo "4. Wyjdź"

    read -p "Wybierz opcję: " option

    case $option in
        1)
            add_group
            ;;
        2)
            add_user
            ;;
        3)
            add_user_to_group
            ;;
        4)
            break
            ;;
        *)
            echo "Nieprawidłowa opcja."
            ;;
    esac

    echo
done
