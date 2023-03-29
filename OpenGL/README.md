# QR_postcards

> **QR_postcards** - это онлайн-редактор новогодних 3D открыток. 

Открытка генерируется с помощью языка разметки "Congratulator" в текстовом документе, который указывается в качестве параметра командной строки.

 ### Пример программ на "Congratulator"

Программа в файле program.txt
```
$ cat program.txt

pine[20,10]
    {
     size=3;
     intense=3;
    }

star[80,80]
    {
     color=blue;
     size=2;
     intense=1;
    }

gift[65,20]
    {
     size=2;
     color=white;
     intense=1;
    }

congrat [52,40]
    {
     color=yellow;
     size=3;
     +"Happy New Year!";
    }

  ```
Открытка 

![example](picture/postcard.png)  


 ### Синтаксис:

Очень простой:

+ Объекты
 - pine
 - star
 - gift
 - congrat

+ Координаты объекта \[x, y\]  

 - x и у от 0 до 100

+ Размер size = n

 - n от 1 до 5 

+ Интенсивность вращения intense = n

 - n от 1 до 5  

+ Цвета color = c

 - с один из (blue, white, yellow, violet, red)  

+ Фон n

 - n от 1 до 5  



-----------------------------------------------------------------------------


 ### Сборка всего проекта:  
```
  $make
```
 Чтобы удалить объектные файлы со всего проекта:  
```
  $make clean
```

 Чтобы запустить *QR_postcard* с программой в файле *program.txt*:
``` 
  $./QR_postcard.exe program.txt
``` 

Для закрытия окна нажать **esc**


### Добавим побольше всего и поменяем фон

![example](picture/postcard2.png)  




