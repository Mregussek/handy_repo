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

### VLANs 2, 3, 4, 5 globally operated, S21 and S22 are used to operate on them

Ustawić hostname w każdym miejscu, stworzyć trunki (int gx/0/x → sw tr en d → sw m t → sw tr nat vlan 99. S21 oraz S22 jako VTP serwer (VTP Mode Server, nadać VTP domain name oraz VTP pass), pozostałe jako VTP Client, S32 jako VTP Transparent

### VLANs 10 and 20 are locally operated on s32

VTP Transparent, zweryfikować poprzez do show vtp status

### Secure the network by restricting propagation of globally operated VLANs to new and not configured roots

Rozumiem to jako utworzenie VTP w każdym urządzeniu i podłączeniu się do jednej tej samej domeny.

```packettracer
switch(config)# vtp domain nazwa_domeny
switch(config)# vtp pass dane_hasło

switch(config)# show vtp status     // sprawdźmy wszędzie stan VTP
```

### In each switch four ports should be assigned to each VLAN available in the switch. These ports will be used to connect computers to the network

Int r g1/0/x-x+3 → sw m acc → sw acc vlan X

### Secure unused ports against unauthorized usage

Możemy je po prostu wyłączyć :D

```packettracer
switch(config-if-range)# sh     // wyłączmy range portów
```

### Configure interfaces used to connect PCs in such way, in which an access to a VLAN other than configured is not possible

Po prostu do interfejsu dołączamu wybrany VLAN.

Patrz przypisywanie VLAN do portu!

Sw m access – działa jak blokada wymuszenia trunku

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
S22(config)# spanning-tree vlan 2 priority 4096
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

Ustawienie etherchannel, na obu stronach linku ustawiamy int range gx/0/x-y, sw trunk enc dot1q,
sw m t, sw tr nat vlan 99, channel-group 2 mode on

### If it is possible, frames assigned to various VLANs should be sent through distinct alternative paths

```packettracer
S32(config)# spanning-tree vlan 2 priority 0
S32(config)# spanning-tree vlan 3 priority 0
```

Trzeba pobawić się korzeniami.

### Transport of VLAN 2 and 3 data to S32 and S33 should not allowed

Switchport trunk i no nie wiem, coś z allowed, to można pytajnikami się dostać.

### All frames with 1234.1234.1234 MAC address must be removed in S23

```packettracer
S23(config)# mac address-table static 1234.1234.1234 vlan 2 int gig0/10 // jest błąd jakiś z interfejsem!
```

## Notatki Lab 14.05

**Dlaczego wpisaliśmy adresy 172.31.1.9 dla PC0 oraz 172.31.1.10 gateway?**

Adresujemy w sposób efektywny:

- dobieramy maskę taką, że wypełniamy ją możliwie największą ilością jedynek, a zwalniamy tyle zer ile potrzebujemy urządzeń!
- adresujemy od największej sieci do najmniejszej!
- adresujemy tak, żeby nie było dziur adresowych (czyli jeśli zakończyliśmy na 7 w większej sieci, to zaczynamy od 8 w mniejszej)!

Adres IP Gateway to adres Routera! Stosujemy to po to, żeby wydostać się z naszej sieci.

W routerze wszystkie porty są domyślne wyłączone!

```packettracer
Router(config)# int gig0/0/0    // załóżmy, że ten chcemy uruchomić
Router(config-if)# no sh
```

**Jak zaadresować Router?**

```packettracer
Router(config)# int gig0/0/0                            // adresujemy w interfejsie!
Router(config-if)# ip add 172.31.1.10 255.255.255.252   // podajemy adres i maskę
Router(config-if)# no sh                                // upewniamy się, że port jest włączony
```

**Dlaczego dioda w przełączniku świeciła na bursztynowo?**

Z powodu spanning-tree. Najpierw stan listening, potem learning.

**O czym należy zawsze pamiętać?**

Jesteśmy proszeni, żeby uzywać hostname, wtedy to ładniej wygląda :D

```packettracer
Router(config)# hostname R1
R1(config)#
```

**Śmieszne połączenie ethernetowe (szeregowe?), jak zaadresować?**

Ponoć jest znącząca różnica pomiędzy gigabithem, a ethernetem (w packet tracer ofc). Dobraliśmy adres 128.1.0.1 255.255.255.252 (dlaczego? bo Pani Anna wybrała :D)

```packettracer
R1(config)# int s0/1/0
R1(config-if)# ip add 128.1.0.1 255.255.255.252
R1(config-if)# no sh    // nie zapominaj o włączeniu portu
```

Ethernet z zasady jest łącze / medium współdzielone, natomiast gigabit to łącze punkt-punkt, więc działają znacząco inaczej! Taki zygzak, to łącze szeregowe, czyli powinno nie być adresów (ale u nas są!). Należy wpisać w google Smart Serial.

Zwróc uwagę, że na końcu łącza mamy symbol zegarka i trzeba wpisać clockrate w tym routerze. Czyli jak mamy narysowany router, obok niego napisany numer portu powiedzmy Se0/1/0 i obok niego zegarek, to tam trzeba wpisać komendę clock rate.

```packettracer
R2(config)# int s0/1/0
R2(config-if)# clock rate <jaka szybkosc>       // na labie wybraliśmy 128000
R2(config-if)# ip add 128.1.0.2 255.255.255.252 // trzeba jeszcze adres ustawić (dlaczego 2 wpisaliśmy?)
```

**Dlaczego nie działał ping z lewej strony (R1) do prawej strony (R2)?**

Sprawdźmy tablicę routingu:

```packettracer
R1# show ip route
```

Więc powiemy ręcznie jak dostać się do innej podsieci.

```packettracer
R1(config)# ip route <podsiec docelowa> <maska docelowa> <przez co mamy dostać się do tej podsieci?>
R1(config)# ip route 172.31.1.0 255.255.255.248 128.1.0.2
```

W naszym przypadku musimy jakoś połączyć się jakoś z podsiecią 172.31.1.0 znajdującej się w R2. Z R2 możemy połączyć się poprzez trasę z połączenia serial portu. Dzięki temu docieramy z R1 do R2. Podajemy 128.1.0.2, ponieważ taki adres przydzieliliśmy portowi na R2, który służy do połączęnia R1 <-> R2.

*Nie zapomnij w tym przypadku o trasie z R2 do R1 (czyli odwrotnej, dlatego ping może nie wracać!).*

Musimy skonfigurować ponownie ip route ze strony R2 do R1, czyli

```packettracer
R2(config)# ip route <podsiec docelowa> <maska docelowa> <przez co mamy dostać się do tej podsieci?>
R2(config)# ip route 172.31.1.8 255.255.255.252 128.1.0.1
```

Tutaj użyliśmy 128.1.0.1, ponieważ na porcie (który łączy R1 z R2) w R1 jest przypisany adres 128.1.0.1. I ping działa!

**Jak podłączyć bezpośrednio podsieć? Ma działać lepiej ale w przypadku braku podłaczenia interfejsów poważne bugi!**

```packettracer
R1(config)# ip route <podsiec docelowa> <maska docelowa> <przez co mamy dostać się do tej podsieci?>
R1(config)# ip route 172.31.1.8 255.255.255.252 s0/1/0
```

```packettracer
R2(config)# ip route <podsiec docelowa> <maska docelowa> <przez co mamy dostać się do tej podsieci?>
R2(config)# ip route 172.31.1.8 255.255.255.252 s0/1/0
```

Przez port s0/1/0 w R1 jest połączeni z R2, oraz przez R2 w s0/1/0 jest połączeni z R1!

## Notatki Lab 28.05

Generalnie nowością był protokół routingu RIP. Zapiszę co z nim było omówione.

Przeszliśmy z konfiguracji interfejsu do konfiguracji routingu ale podejrzewam, że nic się nie stanie jak przejdziemy po prostu z konfiguracji do konfiguracji routingu.

Mamy kilka routerów i chcemy otrzymać do nich dostęp. Nie będziemy tego robić statycznie poprzez *ip route* tylko skorzystamy z protokołu RIP.

```packettracer
Router(config-if)# router rip  
Router(config-router)# version 2            // na lab włączyliśmy wersję 2 (dlaczego?)
Router(config-router)# network 192.168.0.0  // chcemy dać dostęp do tej sieci
```

Teraz mamy dostęp do innych routerów, które również są w sieci 192.168.0.0.

W połączeniu szeregowym nie możemy korzystać z protokołu RIP, należy to zrobić statycznie!

Na końcu zrobiliśmy też tak, że wszystkie trasy statyczne wrzuciliśmy do protokołu RIP. Oczywiście wykonaliśmy te komendy po stronie routera, który podłączony do innych routerów crossem, a do innego serialem. Przypominam - połączenie szeregowe nie możemy skorzystać z RIP! Dlatego po prostu dodamy te ścieżki statyczne do RIPa.

```packettracer
Router(config)# router rip
Router(config-router)# redistribute static      // ponoć działa zawsze!
```

## Notatki Lab 4.06

### Konfiguracja linii terminala oraz komunikacja switch <-> router

Nowością z pewnością jest podanie switchowi adresu bramy (gateway'a), ale po co?

Ogarnąłem, że chcemy pingować switch z routerem, więc na interfejsie switcha, do którego podpięty jest router wykonujemy:

```packettracer
S0(config)# int gig1/0/1
S0(config-if)# ip addr 192.168.0.25 255.255.255.252   // switch ma swój adres
S0(config-if)# exit
S0(config)# ip default-gateway 192.168.0.26           // taki adres będzie miał router
```

Żeby poprawnie skonfigurować połączenie trzeba jeszcze dodać linię wirtualnego terminala:

```packettracer
S0(config)# line vty 0 5        // konfigurujemy 6 linii vty, czyli 6 osób może jednocześnie konfigurować switch
S0(config-line)# password sk    // nadajemy hasło
S0(config-line)# login          // mówimy, żeby korzystał z lokalnej pamięci cache
S0(config-line)# exit
```

Teraz, wchodzimy do switcha i konfigurujemy port, do którego jest podłączony switch.

```packettracer
R0(config)# int g0/0/0
R0(config-if)# ip addr 192.168.0.26 255.255.255.252     // zauważ, że 0.26 był wykorzystany jako default-gateway u góry, dlatego musimy wykorzystać ten adres w routerze.
R0(config-if)# do ping 192.168.0.25                     // ma ping działać!
```

W PC jak wpiszemy komendę telnet możemy połączyć do się do switcha i konfigurować urządzenie z poziomu PC.

```PC
PC> telnet 192.168.0.25         // na chwilę obecną nie działa, bo brakuje jednej komendy w switchu
    password: <tu mamy wpisać nadane hasło>
```

Wchodzimy do switcha i poprawiamy konfigurację (po to żeby umożliwić PCtowi przejście do trybu enable oraz trybu conf t):

```packettracer
S0(config)# enable secret sk        // nadajemy hasło sk
```

### Konfiguracja DHCP

Możemy na początek sprawdzić jakie są konfiguracje:

```packettracer
R0(config)# do show run                     // sprawdźmy, czy jakieś pule DHCP już mamy
R0(config)# no ip dhcp pool <nazwa puli>    // usuńmy, jeśli mamy jakąś niepotrzebną
```

Teraz przechodzimy do routera, w którym chcemy skonfigurować DHCP:

```packettracer
Router(config)# ip dhcp pool vlan_2                         // definiujemy nazwę puli vlan_2
Router(dhcp-config)# network 192.168.0.0 255.255.255.248
Router(dhcp-config)# default-router 192.168.0.3
Router(dhcp-config)# dns-server 8.8.8.8         // serwer google, domyślny serwer dns, który pracuje w sieci
Router(dhcp-config)# exit
Router(config)# ip dhcp pool vlan_3
Router(dhcp-config)# network 192.168.0.0 255.255.255.248
Router(dhcp-config)# default-router 192.168.0.10
Router(dhcp-config)# dns-server 8.8.8.8
Router(dhcp-config)# exit
Router(config)# hostname R3_DHCP                // nadajmy mu nazwę
R3_DHCP(config)#
```

Na zajęciach wyskoczył error DHCP failed. APIPA is being used. O co chodzi? :D

Skonfigurowaliśmy R0, subinterfejs, dodaliśmy dodatkowy adres i nagle zadziało. Zdecydowanie trzeba doczytać temat.

```packettracer
R0(config)# int g0/0/0.1
R0(config-subif)# ip helper-address 192.168.0.21
R0(config-subif)# int g0/0/0.2
R0(config-subif)# int helper-address 192.168.0.21
```

