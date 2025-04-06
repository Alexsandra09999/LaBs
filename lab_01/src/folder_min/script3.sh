#!/bin/bash
output_file="/usr/local/folder_max/output.log"

# Проверяем и создаём файл, если нужно
[ -d "/usr/local/folder_max" ] || sudo mkdir -p "/usr/local/folder_max"
[ -f "$output_file" ] || sudo touch "$output_file"
sudo chown $USER "$output_file"

# Записываем дату
date >> "$output_file"
