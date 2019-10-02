# SelSQL

## Участники
- Хачатрян Илья https://github.com/quiksilver4210
- Пенюгин Антон https://github.com/antons201
- Михайленко Антон https://github.com/aLLnext

## Документация
**SelSQL** – СУБД, разрабатываемая студентами 3 курса ПМИ.

Поддерживаемые команды:

1\.	**CREATE TABLE table_name (col_name col_type constraints, …);**  

Создает новую таблицу. ***table_name*** – название таблицы, ***col_name*** – название колонки, ***col_type*** – тип содержимого колонки таблицы, ***constraints*** – ограничения.

В названии таблицы и колонки (***table_name, col_name***) можно использовать латинские заглавные и строчные буквы, а также цифры. Ограничение по длине – 256 символов(строки большей длины будут обрезаны).

Доступные типы содержимого ячеек таблицы (***col_type***): 
* INT – целочисленные значения, 
* FLOAT – числа с плавающей точкой, 
* CHAR – строка, 
* BOOLEAN – логические значения.

Доступные ограничения (***constraints***):
* NOT NULL – содержимое ячейки в колонке не может быть NULL, 
* UNIQUE - содержимое ячейки в колонке должно быть уникальным, 
* PRIMARY KEY – первичный ключ. Однозначно идентифицирует каждую строку/запись в таблице.

Если PRIMARY KEY указать сразу у нескольких столбцов, то ключ будет составным. 
Названия колонок в одной таблице повторяться не могут. Два раза указывать одно и то же ограничение для одного столбца нельзя. 

2\. **SHOW CREATE TABLE table_name;**

Показывает оператор CREATE TABLE, который будет создавать данную таблицу (с названием ***table_name***).
Ограничение на использование символов в названии таблицы(***table_name***) аналогичны вышеперечисленным.

3\.	**DROP TABLE table_name;**

Удаляет таблицу с названием ***table_name***. 
Ограничение на использование символов в названии таблицы(***table_name***) аналогичны вышеперечисленным.

Вышеперечисленные команды и типы данных необходимо писать в одном регистре (верхнем/нижнем). Примеры:<br>
Можно: **CREATE TABLE table (id int);**<br>
Нельзя: **CReaTe taBle table (id InT);**




**Установка.  
GOOGLE TESTS.**
1. Скачать библиотеку. https://github.com/google/googletest
2. Распаковать в пакет {project_name}/lib

