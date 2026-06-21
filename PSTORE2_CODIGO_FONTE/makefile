EE_BIN = pstore2.elf
EE_OBJS = src/main.o src/menu.o src/usb.o src/graphics.o

#
GSKIT_LIB_DIR = /usr/local/ps2dev/gsKit/lib
PS2SDK_LIB_DIR = $(PS2SDK)/ee/lib
LIB_DIR = /usr/local/ps2dev/ps2sdk/samples/lojaps2/lib

#
EE_INCS = -I./include -I$(PS2SDK)/ee/include -I$(PS2SDK)/common/include -I/usr/local/ps2dev/gsKit/include
EE_LDFLAGS = -L$(LIB_DIR) -L$(PS2SDK_LIB_DIR) -L$(GSKIT_LIB_DIR)

#
EE_LIBS = -Wl,--start-group $(GSKIT_LIB_DIR)/libgskit.a $(GSKIT_LIB_DIR)/libgskit_toolkit.a $(GSKIT_LIB_DIR)/libdmakit.a -lpad -ldebug -lc -lkernel -Wl,--end-group

#
PS2IP = 

all: $(EE_BIN)

#
src/%.o: src/%.c
	ee-gcc $(EE_INCS) -include tamtypes.h -c $< -o $@

$(EE_BIN): $(EE_OBJS)
	ee-gcc -mno-crt0 -T$(PS2SDK)/ee/startup/linkfile -o $@ \
	$(PS2SDK)/ee/startup/crt0.o $(EE_OBJS) $(EE_LDFLAGS) $(EE_LIBS)
	ee-strip --strip-all $@

#
run: all
	ps2client -h $(PS2IP) execee host:$(EE_BIN)

reset:
	ps2client -h $(PS2IP) reset

clean:
	rm -f *.elf src/*.o