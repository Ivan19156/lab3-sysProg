CC      = gcc
CFLAGS  = -Wall -Wextra -g

# Цільовий виконуваний файл
TARGET  = calculator

# Всі об'єктні файли
OBJS    = calculator.tab.o lex.yy.o ast.o

# ── Збірка за замовчуванням ──────────────────
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# ── Генерація парсера (bison) ────────────────
calculator.tab.c calculator.tab.h: calculator.y
	bison -d calculator.y

# ── Генерація лексера (flex) ─────────────────
lex.yy.c: calculator.l calculator.tab.h
	flex calculator.l

# ── Компіляція об'єктних файлів ──────────────
calculator.tab.o: calculator.tab.c
	$(CC) $(CFLAGS) -c calculator.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

# ── Очищення ─────────────────────────────────
clean:
	rm -f $(TARGET) $(OBJS) calculator.tab.c calculator.tab.h lex.yy.c

# ── Запуск ───────────────────────────────────
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
