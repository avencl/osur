include $(POCETNI_DIREKTORIJ)/config.ini

VAR1 = 1
VAR2 = 1
CFLAGS += -D VAR1=$(VAR1) -D VAR2=$(VAR2) -D DIR=$(POCETNI_DIREKTORIJ)

OBJEKTI = malo.o
MODULI = manje/mod-manje.o
MODUL = mod-malo.o

$(MODUL): $(OBJEKTI) $(MODULI)
	$(LD) -r $^ $(LDFLAGS) -o $@ 

malo.o: malo.c $(POCETNI_DIREKTORIJ)/include/postavke.h
	$(CC) $(CFLAGS) -o $@ -c $<
	

.PHONY: $(MODULI)
$(MODULI):
	$(MAKE) -C $(dir $@)

# brisanje
.PHONY: brisi
brisi:
	$(MAKE) -C manje brisi
	rm -f $(OBJEKTI) $(MODUL)