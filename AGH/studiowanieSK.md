# SK Sieci Komputerowe

Włączając urządzenie jesteśmy w trybie użytkownika. Zwany potocznie EXEC pozwala
na przeglądanie informacji, ale nie umożliwia na dokonywanie zmian.

```packettracer
switch>
```

Tryb enable, zwany potocznie uprzywilejowanym ma znacznie większe możliwości.

```packettracer
switch#
```

Następny tryb to tryb konfiguracji, w nim akceptowalne są polecenia konfiguracyjne - polecenia, które mówią co i jak urządzenie ma robić.

```packettracer
switch(config)#
```

## Przydatne pojęcia

### LANy i VLANy

Sieć LAN - obejmuje wszystkie urządzenia w tej samej domenie rozgłoszeniowej.
Bez sieci VLAN switch traktuje wszytstkie swoje interfejsy jak istniejące w jednej domenie rozgłoszoniowej. Czyli, żeby utworzyć dwie domeny rozgłoszeniowe należy kupić dwa różne switche. VLAN pozwala nam na realizację tego zadania bez dodatkowych kosztów.

VLANy tworzy się, żeby:

- zmniejszyć obciążenia procesora,
- zmniejszyć liczbę hostów odbierających ramki (zmniejszamy zagrożenie),
- poprawić bezpieczeństwo hostów,
- utworzyć sieci grupujące użytkowników według działów,
- zmniejszyć obciążenia drzewa rozpinającego (STP)

Sieci VLAN, w których jest wiele połączonych ze sobą switchów, muszą korzystać z magistrali VLAN (VLAN trunking) na łączach między sobą. Używanie trunka powoduje, że na switchach zachodzi proces oznaczania VLAN (VLAN tagging), który polega na dodawaniu nagłówka do ramki przez nadającego switcha. Nagłówek ten zawiera ID, dzięki któremu switch kojarzy, gdzie należy skierować daną ramkę.

Odnośnik do router-on-a-stick -> ponieważ router ma jedno fizyczne łącze do przełącznika LAN, nazywa się go routerem na patyku.

VTP (VLAN Trunking Protocol) - tworzą konfigurację każdej sieci VLAN na jednym switchu (polecenie vlan ID, żeby inne switche dowiedziały się o jej istnieniu). !!! Wiele firm wybiera inne rozwiązanie niż VTP. W książce z certyfikatu CCENT starają się pomijać VTP bo nie ma sensu!!!

Protokół VTP działa w jednym z trzech trybów:

- tryb serwera - jest domyślnym trybem VTP. Jest możliwa edycja sieci VLAN, wersji VTP. Wszelkie zmiany są rozsyłane do innych urządzeń pracujących w sieci.
- tryb transparentny - istnieje możliwość edycji sieci wirtualnych, ale zmiany mają wpływ tylko na lokalny przełącznik. Przełącznik przekazuje ogłoszenia VTP, ale ich nie tworzy, ani nie przetwarza.
- tryb klienta - nie można edytować ustawień sieci VLAN. Informacje o sieciach VLAN są synchronizowane z innymi klientami i serwerami VTP.

### CDP

CDP (Cisco Discovery Protocol) - używany do poznawania topologii sieci w celu przewidywania sposoby normalnej pracy sieci.

Uzyskuje następnujące informacje:

- ID urządzenia,
- adresy sieciowe i łącza danych
- ID portu, z którego wysłano ogłoszenie CDP
- informację, czy jest to router, switch (lista możliwości)
- platforma, informacje o systemie

## Polecenia

### Polecenie CDP (u góry, żeby było blisko definicji)

```packettracer
switch# show cdp neighbors      // informacja zbiorcza o urządzeniach sąsiadujących
switch# show cdp nei detail     // szczegółowe
```

### VTP

Domyślnie każdy pracuje w trybie serwera.

Tutaj konfigurujemy serwer. Musimy skonfigurować trunka!

```packettracer
switch(config-if)# sw m t       // na porcie, na którym nam zależy
switch(config-if)# exit
switch(config)# vtp domain <nazwa domeny>
switch(config)# vtp password <hasło>
```

Konfiguracja klienta:

```packettracer
switch(config-if)# sw m t       // na porcie, na którym nam zależy
switch(config-if)# exit
switch(config)# vtp mode client
switch(config)# vtp domain <nazwa domeny>
switch(config)# vtp password <hasło>
```

### Włącz / Wyłącz

```packettracer
switch(config-if)# sh           // wyłącz / shutdown
switch(config-if)# no sh        // włącz / no shutdown
```

### Analiza stanów

```packettracer
switch# show mac address-table dynamic
switch# show vlan brief
switch# show interfaces
switch# show vtp status
```

### Nadawanie nazwy urządzeniu

```packettracer
switch(config)# hostname S1
S1(config)#
```

### Range interfejsów

```packettracer
switch(config)# int range gig 1/0/13-16         // zwracamy się do portów 13, 14, 15, 16
switch(config-if-range)#
```

### Tworzenie VLAN

```packettracer
switch(config)#                 // musimy być w trybie konfiguracji
switch(config)# vlan 4          // vlan <ID>, zostanie utworzony VLAN z domyślną nazwą VLANxxxx, w tym przypadku VLAN0004
switch(config-vlan)# name UtworzonyVLAN     // nadanie nazwy do VLANu 4
switch(config-vlan)# exit                   // wychodzimy z konfiguracji vlanu
switch(config)#
```

### Przypisywanie VLAN do portu / interfejsu

```packettracer
switch(config)#                     // jesteśmy w trybie konfiguracji
switch(config)# int gig 1/0/2       // dobieramy jeden interfejs, do którego przydzielimy VLAN
switch(config-if)# sw acc vlan <ID> // ustalamy ID vlanu
switch(config-if)# sw m acc         // aby interfejs nie uzgadniał swojego działania jako magistrali, wyłączamy ją
```

### Tworzenie magistrali VLAN (trunka)

Należy wykonać (z obu stron!) polecenia:

```packettracer
switch(config-if)#      // załóżmy, że jesteśmy w porcie, który chcemy konfigurować (po jednej i drugiej stronie!)
switch(config-if)# sw m trunk          // tworzymy magistralę VLAN obsługującą wszystkie sieci VLAN dostępne dla obu switchów
switch(config-if)# sw tr enc dot1q
```

### Zarządzanie VLANami obsługiwanymi przez trunka

Oczywiście konfigurujemy to na interfejsie, na którym nam zależy.

```packettracer
switch(config-if)# sw trunk allowed vlan {add | all | except | remove} <ID>
```

- add - dodaje sieć VLAN do istniejącej listy dozwolonych sieci VLAN
- remove - usuwa z tej listy
- all - oznacza wszystkie sieci VLAN (używane do przywracania ustawień domyślnych)
- except - dodaje do listy wszystkie sieci VLAN, których nie wymieniono w poleceniu

Przykład except:

```packettracer
switch(config-if)# sw trunk allowed vlan except 100-200         // powoduje dodanie sieci VLAN od 1 do 99 oraz od 201 do 4094 do listy sieci dozwolonych na trunku
```

## Notatki Lab 7.05

### Secure the network by restricting propagation of globally operated VLANs to new and not configured roots

Rozumiem to jako utworzenie VTP w każdym urządzeniu i podłączeniu się do jednej tej samej domeny.

```packettracer
switch(config)# vtp domain nazwa_domeny
switch(config)# vtp pass dane_hasło

switch(config)# show vtp status     // sprawdźmy wszędzie stan VTP
```

### In each switch four ports should be assigned to each VLAN available in the switch. These ports will be used to connect computers to the network

### Secure unused ports against unauthorized usage

Możemy je po prostu wyłączyć :D

```packettracer
switch(config-if-range)# sh     // wyłączmy range portów
```

### Configure interfaces used to connect PCs in such way, in which an access to a VLAN other than configured is not possible

Po prostu do interfejsu dołączamu wybrany VLAN.

Patrz przypisywanie VLAN do portu!

### To ports used to connect PCs maximum two computers may be connected. If a third device is attached to the port the port should be switched off. The port may be switched on manually

Należy skonfigurować port-security.

```packettracer
switch(config-if)#      // jesteśmy w interfejsie na jakim nam zależy
switch(config-if)# sw m acc
switch(config-if)# sw port-security maximum 2
switch(config-if)# sw port-security violation shutdown
switch(config-if)# sw port-security mac-address sticky      // coś co nie powinno się tutaj znaleźć?
```

### Any PC connected to a switch should get **immedietaly** an IP address from DHCP server

Kiedy możemy dostać adres natychmiast?

```packettracer
switch(config-if)# spanning-tree portfast
```

### If an interface is used to connect a PC then attachment of a switch to the port should en in switching off such interface

```packettracer
S21(config)# spanning-tree portfast bpduguard default
```

### S21 should be set as a root for all VLANs

Na zajęciach skonfigurowaliśmy dla 2 VLANu.

```packettracer
S21(config)# spanning-tree vlan 2 priority 0
```

### S22 should be set a secondary root for all VLANs

```packettracer
S21(config)# spanning-tree vlan 2 priority 4096
```

### All frames sourced from or destinated to VLAN 99 should be conveyed between switches without any tag

```packettracer
S21(config-if)# sw tru nat vlan 99
```

### Failure of a single connection between bridges should not interrup transport of data even for a short moment

Należy zrobić Etherchannel. 
Robimy drugie połączeniu pomiędzy switchami.
Pomiędzy portami, którymi podłączyliśmy switche wykonujemy:

Załóżmy, że switche S23 (gig1/0/4 oraz gig1/0/6) oraz S31 (gig1/0/4 oraz gig1/0/6) są połączone.

```packettracer
S23(config)# int ra g1//0/4, g1/0/6
S23(config-if)# sw tr enc dot
S23(config-if)# sw mod tru
S23(config-if)# sw tru nat vlan 99
S23(config-if)# channel-group 2 mode on
```

```packettracer
S32(config)# int ra g1//0/4, g1/0/6
S32(config-if)# sw tr enc dot
S32(config-if)# sw mod tru
S32(config-if)# sw tru nat vlan 99
S32(config-if)# channel-group 2 mode on
```

### If it is possible, frames assigned to various VLANs should be sent through distinct alternative paths

I chyba transparent???

```packettracer
S32(config)# spanning-tree vlan 2 priority 0
S32(config)# spanning-tree vlan 3 priority 0
```

### Transport of VLAN 2 and 3 data to S32 and S33 should not allowed

### All frames with 1234.1234.1234 MAC address must be removed in S23

```packettracer
S23(config)# mac address-table static 1234.1234.1234 vlan 2 int gig0/10 // jest błąd jakiś z interfejsem!
```
