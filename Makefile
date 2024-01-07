CC = gcc
CFLAGS = -Wall -Wextra -pthread
SRC_DIR = src
BUILD_DIR = build
SERVER_TARGET = server
CLIENT_TARGET = client

all: $(SERVER_TARGET) $(CLIENT_TARGET)

#linkovanie $@ nazov premennej, &^ vsetky zavislosti
$(SERVER_TARGET): $(BUILD_DIR)/server.o
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT_TARGET): $(BUILD_DIR)/client.o
	$(CC) $(CFLAGS) -o $@ $^

#vytvorenie objektovych suborov
$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c
#prikaz na kompilaciu server.c na server.o
# -c nevykonaj linkovanie
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)/*.o $(SERVER_TARGET) $(CLIENT_TARGET)