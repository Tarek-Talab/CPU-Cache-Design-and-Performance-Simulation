#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int control;      // Valid bit (Geçerlilik biti) 
    unsigned int tag; // Tag etikten alanı
    int lru_counter;  // LRU en son kullanılan sayacı
} CacheLine;

// Fonksiyon Prototipleri
void adresCozumle(unsigned int address, int cache_size, int block_size, int associativity, CacheLine* cache);
void cacheKontrol(CacheLine* cache, unsigned int index, unsigned int tag, int way_sayisi);
void raporla(int t1, int t2, int way_sayisi, int toplam_satir);
int log2Hesapla(int sayi);
void formulleriGoster();
int hit_Counter = 0;
int miss_Counter = 0;

int main() {
    int cache_size, block_size, associativity;
    int t1, t2; // AMAT hesaplaması için gecikme süreleri 

    // Parametrik yapılandırma girişi
    printf("Onbellek Boyutunu Girin (Byte): "); 
    scanf("%d", &cache_size);
    printf("Blok Boyutunu Girin (Byte): ");
    scanf("%d", &block_size);
    if ((cache_size & (cache_size - 1)) != 0 || (block_size & (block_size - 1)) != 0) {
    printf("Hata: Onbellek veya Blok boyutlari 2'nin kuvveti olmalidir!\n");
    return 0;
}   
    printf("iliskilendirme Derecesini (Way Sayisi) Girin: ");
    scanf("%d", &associativity);
    // associativity scan edildikten hemen sonra:


    int toplam_satir = cache_size / block_size;
    //Dinamik bellek yönetimi (malloc) 
    CacheLine* cache = (CacheLine*) malloc(toplam_satir * sizeof(CacheLine));
    //sıfırlama yada temizleme for döngüsü 
    for(int i = 0; i < toplam_satir; i++) {
        cache[i].control = 0;
        cache[i].tag = 0;
        cache[i].lru_counter = 0;
    }

    // main içinde associativity scan edildikten sonra eklenecek:
    if (associativity == 1) {
        printf("Sistem Mesaji: DIRECT MAPPING (Dogrudan Esleme) modu aktif.\n");
    } 
    else if (associativity > 1 && associativity < toplam_satir) {
        printf("Sistem Mesaji: %d-WAY SET-ASSOCIATIVE modu aktif.\n", associativity);
    } 
    else {
        // Kullanıcı toplam_satir'a eşit veya daha büyük bir değer girerse
        printf("\n------------------------------------------------------------\n");
        printf("UYARI: Girdiginiz deger (%d) toplam satir sinirina (%d) ulasti veya gecti.\n", associativity, toplam_satir);
        printf("Sistem otomatik olarak FULLY ASSOCIATIVE (Tam Iliskili) moda alindi.\n");
        printf("------------------------------------------------------------\n");
        
        associativity = toplam_satir; 
    }

    // AMAT parametreleri
    printf("Cache Erisim Suresi (t1 - ns): "); 
    scanf("%d", &t1);
    printf("RAM Erisim Suresi (t2 - ns): "); 
    scanf("%d", &t2);

    

    unsigned int okunanAdres;
    FILE *dosya = fopen("trace.txt", "r");
    if (dosya == NULL) {
        printf("Hata: trace.txt dosyasi bulunamadi!\n");
        return 1;
    }

    printf("\nSimulasyon Calisiyor...\n");
    while (fscanf(dosya, "%x", &okunanAdres) != EOF) {
        adresCozumle(okunanAdres, cache_size, block_size, associativity, cache);
    }
    fclose(dosya);

    raporla(t1, t2, associativity, toplam_satir); // Nihai raporlama

    char secim;
    printf("\nTeorik formullere goz atmak ister misiniz? (e/h): ");
    scanf(" %c", &secim);

    if (secim == 'e' || secim == 'E') {
        formulleriGoster();
    }

    free(cache);
    return 0;   
}

void adresCozumle(unsigned int address, int cache_size, int block_size, int associativity, CacheLine* cache) {
    // Stallings adres ayrıştırma modelleri
    int offset_Bit_number = log2Hesapla(block_size); 
    int set_numbers = (associativity == 0) ? 1 : (cache_size / (block_size * associativity)); 
    int indexBit_number = (associativity == 0) ? 0 : log2Hesapla(set_numbers);

    // Bit düzeyinde maskeleme ve kaydırma işlemleri 
    unsigned int offset = address & ((1 << offset_Bit_number) - 1); 
    unsigned int index = (address >> offset_Bit_number) & ((1 << indexBit_number) - 1); 
    unsigned int tag = address >> (offset_Bit_number + indexBit_number);

    // Analiz edilen değerleri kontrol modülüne gönderiyoruz
    cacheKontrol(cache, index, tag, associativity);
}

void cacheKontrol(CacheLine* cache, unsigned int index, unsigned int tag, int way_sayisi) {
    int hit = 0;
    int way_start = index * way_sayisi; // İlgili set'in başlangıç noktası

    // Küme içindeki yolları tara (n-Way Search)
    for (int i = 0; i < way_sayisi; i++) {
        if (cache[way_start + i].control == 1 && cache[way_start + i].tag == tag) {
            hit_Counter++;
            hit = 1;
            // Zaman damgasını güncelle 
            cache[way_start + i].lru_counter = hit_Counter + miss_Counter; 
            break;
        }
    }

    if (!hit) {
        miss_Counter++;
        // LRU Yer Değiştirme Algoritması 
        int lru_index = way_start;
        for (int i = 1; i < way_sayisi; i++) {
            if (cache[way_start + i].lru_counter < cache[way_start + lru_index].lru_counter)
                lru_index = way_start + i;
        }
        cache[lru_index].control = 1;
        cache[lru_index].tag = tag;
        cache[lru_index].lru_counter = hit_Counter + miss_Counter;
    }
}

void raporla(int t1, int t2, int way_sayisi, int toplam_satir) {
    float hit_rate = (float)hit_Counter / (hit_Counter + miss_Counter);
    float amat = t1 + ((1.0 - hit_rate) * t2); 

    printf("\n============================================");
    printf("\n        SIMULASYON NIHAI RAPORU             ");
    printf("\n============================================");
    
    // Mimarinin Ne Olduğunu Yazdır
    if (way_sayisi == 1) printf("\nMimari Turu      : Direct Mapping");
    else if (way_sayisi >= toplam_satir) printf("\nMimari Turu      : Fully Associative");
    else printf("\nMimari Turu      : %d-Way Set-Associative", way_sayisi);

    printf("\nToplam Erisimi   : %d", hit_Counter + miss_Counter); 
    printf("\nToplam Hit       : %d", hit_Counter); 
    printf("\nToplam Miss      : %d", miss_Counter); 
    printf("\nHit Orani        : %%%.2f", hit_rate * 100); 
    printf("\nMiss Orani       : %%%.2f", (1.0 - hit_rate) * 100); 
    printf("\nAMAT (Gecikme)   : %.2f ns", amat); 
    printf("\n============================================\n");
}

int log2Hesapla(int sayi) {
    int sonuc = 0;
    while (sayi >>= 1) sonuc++;
    return sonuc;
}

void formulleriGoster() {
    printf("\n========================================================");
    printf("\n   WILLIAM STALLINGS - MIMARI ANALIZ REFERANSI          ");
    printf("\n========================================================");
    printf("\n1. ADRES COZUMLEME (Bitwise Ayristirma):");
    printf("\n   - Offset Bits = log2(Blok Boyutu)");
    printf("\n   - Index Bits  = log2(Set Sayisi)");
    printf("\n   - Tag Bits    = 32 - (Index + Offset)");
    
    printf("\n\n2. PERFORMANS METRIKLERI:");
    printf("\n   - Hit Rate (h) = Toplam Hit / Toplam Erisim");
    printf("\n   - Miss Rate    = 1 - Hit Rate");
    printf("\n   - AMAT (Ta)    = T1 + (Miss Rate * T2)");
    
    printf("\n\n3. ESLEME KURALLARI (Mapping Rules):");
    printf("\n   - Direct Mapping: Way = 1 ise her blok tek satira gider.");
    printf("\n   - Set-Associative: n-Way ise bloklar kume icinde n yere gidebilir.");
    printf("\n   - Fully Associative: Way = Toplam Satir ise blok her yere gidebilir.");
    printf("\n========================================================\n");
}