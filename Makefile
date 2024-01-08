CC = gcc
#-Wall a -Wextra zobrazuje varovania
CFLAGS = -Wall -Wextra -pthread
SRC_DIR = src
BUILD_DIR = build
SERVER_TARGET = server
CLIENT_TARGET = client

# Zoznam všetkých súborov pre server a klient
SERVER_SOURCES = $(SRC_DIR)/server.c $(SRC_DIR)/Game.c $(SRC_DIR)/Snake.c $(SRC_DIR)/Position.c $(SRC_DIR)/Food.c $(SRC_DIR)/Direction.h $(SRC_DIR)/Board.c
CLIENT_SOURCES = $(SRC_DIR)/client.c $(SRC_DIR)/Game.c $(SRC_DIR)/Snake.c $(SRC_DIR)/Position.c $(SRC_DIR)/Food.c $(SRC_DIR)/Direction.h $(SRC_DIR)/Board.c

# Zoznam všetkých objektových súborov pre server a klient
SERVER_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SERVER_SOURCES))
CLIENT_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CLIENT_SOURCES))

all: $(SERVER_TARGET) $(CLIENT_TARGET)

#linkovanie $@ nazov premennej, &^ vsetky zavislosti
$(SERVER_TARGET): $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT_TARGET): $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

#pravidlo na vytvorenie objektoveho suboru na zaklade zdrojoveho suboru
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
#Kompiluje zdrojový súbor do objektového súboru s názvom $@ a využíva prvú závislosť ($<), čo je zdrojový súbor
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)/*.o $(SERVER_TARGET) $(CLIENT_TARGET)
