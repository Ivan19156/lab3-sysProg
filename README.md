# Lab 3 — Lex/YACC Arithmetic Expression Parser

Лабораторна робота з системного програмування.  
Лексичний аналізатор і синтаксичний парсер арифметичних виразів на основі **Flex** і **Bison** з побудовою AST та візуалізацією через d3.js.

## Можливості

- Арифметичні операції: `+`, `-`, `*`, `/`
- Унарний мінус: `-x`
- Дужки: `(2 + 3) * 4`
- Змінні та присвоєння: `x = 5`
- Числа з плаваючою крапкою: `3.14`
- Виведення AST у термінал
- Генерація JSON для візуалізації дерева

## Структура

```
lab3/
├── ast.h              # Визначення типів вузлів AST
├── ast.c              # Реалізація: створення вузлів, eval(), ast_to_json()
├── calculator.l       # Flex: лексер (токени)
├── calculator.y       # Bison: граматика і побудова AST
├── Makefile
└── visualizer/
    └── index.html     # Інтерактивна візуалізація AST через d3.js
```

## Збірка

```bash
make
```

Потрібно: `flex`, `bison`, `gcc`

```bash
sudo apt install flex bison gcc
```

## Запуск

```bash
./calculator
```

Або через pipe:

```bash
echo '(2 + 3) * 4' | ./calculator
echo 'x = 5; x + 2.5' | ./calculator
echo '-(3 + 1) / 2' | ./calculator
```

## Приклад виводу

```
$ echo '(2 + 3) * 4' | ./calculator
= 20
AST:
  MUL
    ADD
      NUMBER(2)
      NUMBER(3)
    NUMBER(4)
{"name":"*","children":[{"name":"+","children":[{"name":"2"},{"name":"3"}]},{"name":"4"}]}
```

## Візуалізація AST

1. Отримати JSON з калькулятора:
```bash
echo '(2 + 3) * 4' | ./calculator | tail -1
```
2. Відкрити `visualizer/index.html` у браузері
3. Вставити JSON у поле зліва → натиснути **Render**

## Технології

| Компонент | Інструмент |
|-----------|-----------|
| Лексер    | Flex      |
| Парсер    | Bison (LALR(1)) |
| AST       | C (рекурсивні структури) |
| Візуалізація | d3.js v7 |
