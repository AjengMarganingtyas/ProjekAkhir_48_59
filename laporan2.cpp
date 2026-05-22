#include <iostream>
#include <stdio.h>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

using namespace std;

//Double Linked List
struct Foto {
    string nama, tanggal;
    Foto* next;
    Foto* prev;
};

Foto* dHead = NULL;
Foto* dTail = NULL;
Foto* current = NULL;

//Single Linked List
struct Node {
    string nama, tanggal;
    Node* next;
};

Node* sHead = NULL;
Node* sTail = NULL;

//Menambahkan Foto ke Favorit
struct Favorit {
    string nama, tanggal;
    Favorit* next;
    Favorit* prev;
};

Favorit* favHead = NULL;
Favorit* favTail = NULL;
Favorit* favCurrent = NULL;

bool isLoading = false;

//Auoto Tanggal
string getTanggalHariIni() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    sprintf(buffer,
            "%04d-%02d-%02d",
            1900 + ltm->tm_year,
            1 + ltm->tm_mon,
            ltm->tm_mday);
    return string(buffer);
}

//Lowercase
string toLowerCase(string text) {
    transform(text.begin(),
              text.end(),
              text.begin(),
              ::tolower);
    return text;
}

//Cek Dublikat Foto
bool cekNamaSama(string nama) {
    Node* temp = sHead;

    while (temp) {
        if (toLowerCase(temp->nama) ==
            toLowerCase(nama)) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

//Simpan Foto ke File
void simpanFile() {
    FILE* file = fopen("galeri.txt", "w");
    if (file == NULL)
        return;
    Node* temp = sHead;
    while (temp) {
        fprintf(file,
                "%s|%s\n",
                temp->nama.c_str(),
                temp->tanggal.c_str());
        temp = temp->next;
    }
    fclose(file);
}

//Simpan Foto Favorit ke File
void simpanFavorit() {
    FILE* file = fopen("favorit.txt", "w");
    if (file == NULL)
        return;
    Favorit* temp = favHead;
    while (temp) {
        fprintf(file,
                "%s|%s\n",
                temp->nama.c_str(),
                temp->tanggal.c_str());
        temp = temp->next;
    }
    fclose(file);
}

//Tambah Data Foto
void tambahData(string nama, string tanggal) {
    if (nama.empty()) {
        cout << "Nama tidak boleh kosong !!\n";
        return;
    }
    if (cekNamaSama(nama)) {
        cout << "Nama foto sudah ada !!\n";
        return;
    }

    Node* n = new Node;
    n->nama = nama;
    n->tanggal = tanggal;
    n->next = NULL;

    if (!sHead)
        sHead = sTail = n;
    else {
        sTail->next = n;
        sTail = n;
    }

    Foto* f = new Foto;
    f->nama = nama;
    f->tanggal = tanggal;
    f->next = NULL;
    f->prev = NULL;

    if (!dHead)
        dHead = dTail = current = f;
    else {
        dTail->next = f;
        f->prev = dTail;
        dTail = f;
    }
    if (!isLoading)
        simpanFile();
}

//Load File
void loadFile() {
    FILE* file = fopen("galeri.txt", "r");
    if (file == NULL)
        return;
    isLoading = true;
    char data[200];
    while (fgets(data, sizeof(data), file)) {
        string line = data;
        line.erase(remove(line.begin(),
                          line.end(),
                          '\n'),
                   line.end());
        int posisi = line.find('|');
        string nama = line.substr(0, posisi);
        string tanggal = line.substr(posisi + 1);
        tambahData(nama, tanggal);
    }
    isLoading = false;
    fclose(file);
}

//Load Favorit
void loadFavorit() {
    FILE* file = fopen("favorit.txt", "r");
    if (file == NULL)
        return;
    char data[200];
    while (fgets(data, sizeof(data), file)) {
        string line = data;
        line.erase(remove(line.begin(),
                          line.end(),
                          '\n'),
                   line.end());
        int posisi = line.find('|');
        string nama = line.substr(0, posisi);
        string tanggal = line.substr(posisi + 1);

        Favorit* baru = new Favorit;

        baru->nama = nama;
        baru->tanggal = tanggal;
        baru->next = NULL;
        baru->prev = NULL;

        if (!favHead)
            favHead = favTail = favCurrent = baru;
        else {
            favTail->next = baru;
            baru->prev = favTail;
            favTail = baru;
        }
    }
    fclose(file);
}

//Tampilkan Data Foto
void tampilData() {
    Node* temp = sHead;
    cout << "\n=======================================================\n";
    cout << "|                     GALERI FOTO                     |\n";
    cout << "=======================================================\n";
    cout << "| "
         << left << setw(5) << "No"
         << "| "
         << setw(30) << "Nama Foto"
         << "| "
         << setw(13) << "Tanggal"
         << "|\n";
    cout << "-------------------------------------------------------\n";

    if (!temp) {
        cout << "| "
             << left << setw(58)
             << "File kosong"
             << "|\n";
        cout << "-------------------------------------------------------\n";
        return;
    }

    int no = 1;
    while (temp) {
        cout << "| "
             << left << setw(5) << no++
             << "| "
             << setw(30) << temp->nama
             << "| "
             << setw(13) << temp->tanggal
             << "|\n";
        temp = temp->next;
    }
    cout << "-------------------------------------------------------\n";
}

//Search Nama Foto
void searchNama(string key) {
    Node* temp = sHead;
    bool found = false;
    key = toLowerCase(key);
    
    while (temp) {
        string namaTemp = toLowerCase(temp->nama);
        if (namaTemp.find(key) != string::npos) {
            cout << "\nFoto ditemukan !!\n";
            cout << "Nama     : "
                 << temp->nama
                 << endl;
            cout << "Tanggal  : "
                 << temp->tanggal
                 << endl << endl;
            found = true;
        }
        temp = temp->next;
    }
    if (!found)
        cout << "Data tidak ditemukan !!\n";
}

//Search Tanggal Foto
void searchTanggal(string key) {
    Node* temp = sHead;
    bool found = false;
    
    while (temp) {
        if (temp->tanggal == key) {
            cout << "\nFoto ditemukan !!\n";
            cout << "Nama     : "
                 << temp->nama
                 << endl;
            cout << "Tanggal  : "
                 << temp->tanggal
                 << endl << endl;
            found = true;
        }
        temp = temp->next;
    }
    if (!found)
        cout << "Data tidak ditemukan !!\n";
}

//Sorting
void sortNamaAsc() {
    for (Node* i = sHead; i; i = i->next)
        for (Node* j = i->next; j; j = j->next)
            if (i->nama > j->nama) {
                swap(i->nama, j->nama);
                swap(i->tanggal, j->tanggal);
            }
    tampilData();
}

void sortNamaDesc() {
    for (Node* i = sHead; i; i = i->next)
        for (Node* j = i->next; j; j = j->next)
            if (i->nama < j->nama) {
                swap(i->nama, j->nama);
                swap(i->tanggal, j->tanggal);
            }
    tampilData();
}

void sortTanggalAsc() {
    for (Node* i = sHead; i; i = i->next)
        for (Node* j = i->next; j; j = j->next)
            if (i->tanggal > j->tanggal) {
                swap(i->nama, j->nama);
                swap(i->tanggal, j->tanggal);
            }
    tampilData();
}

void sortTanggalDesc() {
    for (Node* i = sHead; i; i = i->next)
        for (Node* j = i->next; j; j = j->next)
            if (i->tanggal < j->tanggal) {
                swap(i->nama, j->nama);
                swap(i->tanggal, j->tanggal);
            }
    tampilData();
}

//Edit Foto
void editFoto(string namaLama, string namaBaru) {
    if (cekNamaSama(namaBaru)) {
        cout << "Nama foto baru sudah ada !!\n";
        return;
    }

    Node* temp = sHead;
    bool found = false;

    while (temp) {
        if (temp->nama == namaLama) {
            temp->nama = namaBaru;
            found = true;
            break;
        }
        temp = temp->next;
    }

    Foto* f = dHead;

    while (f) {
        if (f->nama == namaLama) {
            f->nama = namaBaru;
        }
        f = f->next;
    }

    Favorit* fav = favHead;

    while (fav) {
        if (fav->nama == namaLama) {
            fav->nama = namaBaru;
        }
        fav = fav->next;
    }
    if (found) {
        cout << "Nama foto berhasil diubah !!\n";
        simpanFile();
        simpanFavorit();
    }
    else {
        cout << "Foto tidak ditemukan !!\n";
    }
}

//Tambah Favorit
void tambahFavorit(string key) {
    Node* temp = sHead;
    
    while (temp && temp->nama != key)
        temp = temp->next;
    if (!temp) {
        cout << "Foto tidak ditemukan !!\n";
        return;
    }

    Favorit* cek = favHead;
    while (cek) {
        if (cek->nama == key) {
            cout << "Foto sudah ada di favorit !!\n";
            return;
        }
        cek = cek->next;
    }

    Favorit* baru = new Favorit;
    baru->nama = temp->nama;
    baru->tanggal = temp->tanggal;
    baru->next = NULL;
    baru->prev = NULL;

    if (!favHead)
        favHead = favTail = favCurrent = baru;
    else {
        favTail->next = baru;
        baru->prev = favTail;
        favTail = baru;
    }
    cout << "Foto berhasil ditambahkan ke favorit !!\n";
    simpanFavorit();
}

//Tampilkan Data Favorit
void tampilFavorit() {
    Favorit* temp = favHead;
    cout << "\n=============== FOTO FAVORIT ===============\n";
    if (!temp) {
        cout << "Belum ada foto favorit !!\n";
        return;
    }
    cout << "--------------------------------------------\n";
    cout << "| "
         << left << setw(26)
         << "Nama Foto"
         << "| "
         << setw(13)
         << "Tanggal"
         << "|\n";
    cout << "--------------------------------------------\n";

    while (temp) {
        cout << "| "
             << left << setw(26)
             << temp->nama
             << "| "
             << setw(13)
             << temp->tanggal
             << "|\n";
        temp = temp->next;
    }
    cout << "--------------------------------------------\n";
}

//Random Foto
void randomFoto() {
    if (!sHead) {
        cout << "Galeri kosong !!\n";
        return;
    }

    int jumlah = 0;
    Node* temp = sHead;
    while (temp) {
        jumlah++;
        temp = temp->next;
    }

    int randomIndex = rand() % jumlah;
    temp = sHead;
    for (int i = 0; i < randomIndex; i++)
        temp = temp->next;
    cout << "\n========== SURPRISE FOTO ==========\n";
    cout << "Nama Foto : "
         << temp->nama
         << endl;
    cout << "Tanggal   : "
         << temp->tanggal
         << endl;
}

//Foto Terbaru
void fotoTerbaru() {
    if (!dTail) {
        cout << "Galeri kosong !!\n";
        return;
    }
    cout << "\n========== FOTO TERBARU ==========\n";
    cout << "Nama Foto : "
         << dTail->nama
         << endl;
    cout << "Tanggal   : "
         << dTail->tanggal
         << endl;
}

//Statistik Foto
void statistikFoto() {
    int totalFoto = 0;
    int totalFavorit = 0;
    Node* temp = sHead;
    
    while (temp) {
        totalFoto++;
        temp = temp->next;
    }

    Favorit* fav = favHead;
    while (fav) {
        totalFavorit++;
        fav = fav->next;
    }

    cout << "\n============= STATISTIK FOTO =============\n";
    cout << "Total Foto            : "
         << totalFoto
         << endl;
    cout << "Total Foto Favorit    : "
         << totalFavorit
         << endl;
         
    if (totalFoto == 0)
        cout << "Status Galeri         : Kosong\n";
    else
        cout << "Status Galeri         : Ada isi\n";
    if (dHead) {
        cout << "Foto Pertama          : "
             << dHead->nama
             << endl;
    }
    if (dTail) {
        cout << "Foto Terakhir         : "
             << dTail->nama
             << endl;
    }
    cout << "==========================================\n";
}

//Hapus Foto
void hapusFoto(string key) {
    Node* curr = sHead;
    Node* prev = NULL;

    while (curr && curr->nama != key) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) {
        cout << "Foto tidak ditemukan !!\n";
        return;
    }

    // ===== KONFIRMASI =====
    char pilih;
    cout << "Yakin ingin menghapus foto \""
         << curr->nama
         << "\" ? (y/t): ";
    cin >> pilih;

    if (pilih != 'y' && pilih != 'Y') {
        cout << "Penghapusan dibatalkan !!\n";
        return;
    }

    //Hapus Single Linked List
    if (!prev)
        sHead = curr->next;
    else
        prev->next = curr->next;
    if (curr == sTail)
        sTail = prev;
    delete curr;

    //Hapus Double Linked List
    Foto* f = dHead;
    
    while (f && f->nama != key)
        f = f->next;
    if (f) {
        if (f == dHead)
            dHead = f->next;
        if (f == dTail)
            dTail = f->prev;
        if (f->prev)
            f->prev->next = f->next;
        if (f->next)
            f->next->prev = f->prev;
        if (current == f)
            current = f->next ? f->next : f->prev;
        delete f;
    }
    cout << "Foto berhasil dihapus !!\n";
    simpanFile();
}

//Viewer Foto
void lihatCurrent() {
    if (!current) {
        cout << "Tidak ada foto !!\n";
        return;
    }

    cout << "\n----------------------------------\n";
    cout << "| "
         << left << setw(17)
         << "Nama Foto"
         << "| "
         << setw(13)
         << "Tanggal"
         << "|\n";
    cout << "----------------------------------\n";
    cout << "| "
         << left << setw(17)
         << current->nama
         << "| "
         << setw(13)
         << current->tanggal
         << "|\n";
    cout << "----------------------------------\n";
}

void viewerMenu() {
    if (!dHead) {
        cout << "Galeri kosong !!\n";
        return;
    }
    if (!current)
        current = dHead;

    int pilih;
    do {
        lihatCurrent();
        cout << "\n1. Next\n";
        cout << "2. Previous\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        if (pilih == 1) {
            if (current->next)
                current = current->next;
            else
                cout << "Sudah di foto terakhir !!\n";
        }
        else if (pilih == 2) {
            if (current->prev)
                current = current->prev;
            else
                cout << "Sudah di foto pertama !!\n";
        }
    } while (pilih != 0);
}

// Viewer Favotit
void lihatFavoritCurrent() {
    if (!favCurrent) {
        cout << "Tidak ada foto favorit !!\n";
        return;
    }

    cout << "\n----------------------------------\n";
    cout << "| "
         << left << setw(17)
         << "Nama Foto"
         << "| "
         << setw(13)
         << "Tanggal"
         << "|\n";
    cout << "----------------------------------\n";
    cout << "| "
         << left << setw(17)
         << favCurrent->nama
         << "| "
         << setw(13)
         << favCurrent->tanggal
         << "|\n";
    cout << "----------------------------------\n";
}

void viewerFavorit() {
    if (!favHead) {
        cout << "Belum ada foto favorit !!\n";
        return;
    }
    if (!favCurrent)
        favCurrent = favHead;
    int pilih;

    do {
        lihatFavoritCurrent();
        cout << "\n1. Next\n";
        cout << "2. Previous\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        if (pilih == 1) {
            if (favCurrent->next)
                favCurrent = favCurrent->next;
            else
                cout << "Sudah di favorit terakhir !!\n";
        }
        else if (pilih == 2) {
            if (favCurrent->prev)
                favCurrent = favCurrent->prev;
            else
                cout << "Sudah di favorit pertama !!\n";
        }
    } while (pilih != 0);
}

//Main Function
int main() {
    srand(time(0));
    int pilih, sub;
    string nama, tanggal, namaBaru;
    loadFile();
    loadFavorit();

    do {
        cout << "\n============ MENU GALERI FOTO ==============\n";
        cout << "1. Tambah Foto\n";
        cout << "2. Tampilkan Semua\n";
        cout << "3. Searching\n";
        cout << "4. Sorting\n";
        cout << "5. Edit Nama Foto\n";
        cout << "6. Hapus Foto\n";
        cout << "7. Viewer Foto\n";
        cout << "8. Tambah Favorit\n";
        cout << "9. Tampilkan Favorit\n";
        cout << "10. Surprise Me\n";
        cout << "11. Foto Terbaru\n";
        cout << "12. Viewer Favorit\n";
        cout << "13. Statistik Foto\n";
        cout << "0. Keluar\n\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: {
                int jumlah;
                cout << "Ingin menambah berapa foto : ";
                cin >> jumlah;
                cin.ignore();
                if (jumlah <= 0) {
                    cout << "Jumlah tidak valid !!\n";
                    break;
                }
                for (int i = 1; i <= jumlah; i++) {
                    cout << "\nFoto ke-"
                         << i
                         << endl;
                    cout << "Nama: ";
                    getline(cin, nama);
                    tanggal = getTanggalHariIni();
                    cout << "Tanggal otomatis: "
                         << tanggal
                         << endl;
                    tambahData(nama, tanggal);
                }
                break;
            }

            case 2:
                tampilData();
                break;

            case 3:
                cout << "\n1. Cari Nama\n";
                cout << "2. Cari Tanggal\n";
                cout << "Pilih: ";
                cin >> sub;
                cin.ignore();
                if (sub == 1) {
                    cout << "Masukkan nama: ";
                    getline(cin, nama);
                    searchNama(nama);
                }
                else if (sub == 2) {
                    cout << "Masukkan tanggal: ";
                    getline(cin, tanggal);
                    searchTanggal(tanggal);
                }
                break;

            case 4:
                cout << "\n1. Nama ASC\n";
                cout << "2. Nama DESC\n";
                cout << "3. Tanggal ASC\n";
                cout << "4. Tanggal DESC\n";
                cout << "Pilih: ";
                cin >> sub;

                if (sub == 1)
                    sortNamaAsc();
                else if (sub == 2)
                    sortNamaDesc();
                else if (sub == 3)
                    sortTanggalAsc();
                else if (sub == 4)
                    sortTanggalDesc();
                break;
                
            case 5:
				cin.ignore();
				cout << "Nama lama: ";
				getline(cin, nama);
				cout << "Nama baru: ";
				getline(cin, namaBaru);
				editFoto(nama, namaBaru);
				break;
            
            case 6:
                cin.ignore();
                cout << "Masukkan nama foto: ";
                getline(cin, nama);
                hapusFoto(nama);
                break;

            case 7:
                viewerMenu();
                break;

            case 8:
                cin.ignore();
                cout << "Masukkan nama foto: ";
                getline(cin, nama);
                tambahFavorit(nama);
                break;

            case 9:
                tampilFavorit();
                break;

            case 10:
                randomFoto();
                break;

            case 11:
                fotoTerbaru();
                break;

            case 12:
                viewerFavorit();
                break;

            case 13:
                statistikFoto();
                break;
        }
    } while (pilih != 0);
    return 0;
}
