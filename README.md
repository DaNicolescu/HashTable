HashTable  
	hash.h contine definirea structurii TH (Hash table-ul) si prototipurile
functiilor. lista.h contine typedef-urile pentru pointerii la functii si
definirea structurilor TCelulaG si TInfo. functions.c contine definirea
functiilor, iar hash.c contine int main-ul.  
    In hash.c se realizeaza citirea din fisier si apelarea functiilor
corespunzatoare.  
    Functia IniTH aloca memorie pentru Hash table si atribuie pointerii
la functiile hash, de comparare a cheilor, de afisare si eliberare a cheii 
structurii TH. Pointerii la functii depind de tipul cheii, acestia fiind 
determinati in main.  
	Functia InsTH insereaza un element in Hash table. Aceasta stabileste
bucket-ul corespunzator cheii citite, apoi apeleaza functia InsLG.  
	Functia InsLG insereaza un element intr-o lista generica. Mai intai
se aloca un element cu dimensiuni determinate de parametrii functiei, apoi
se copiaza informatia de la a_val si a_key (adresa valorii, adresa cheii)
in elementul alocat. De asemenea se atribuie pointerii corespunzatori afisarii
si eliberarii valorii. Dupa formarea elementului, se determina daca acesta
trebuie sa inlocuiasca un alt element sau trebuie inserat la sfarsitul listei,
apoi se realizeaza operatia.  
	Functia ResTH redimensioneaza Hash tabel-ul in cazul in care a fost
depasit load factor-ul si repozitioneaza elementele deja existente in functie 
de noul cod hash al cheii fiecaruia.  
	Functia find_value_by_key se pozitioneaza in bucket-ul corespunzator
cheii date ca parametru, apoi o cauta in lista cu ajutorul functiei
de comparare a cheilor. Daca o gaseste, afiseaza valoarea cheii corespunzatoare,
altfel, afiseaza ca nu exista.  
	Functia del_by_key este asemanatoare cu find_value_by_key, insa aceasta
elimina elementul ce are cheia data ca paramentru.  
	Functia free_hash elibereaza Hash tabel-ul din memorie, iar
show_int_key, show_string_key, show_mat_value, show_stud_value si Print_Hash 
se folosesc la afisare.
