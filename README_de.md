[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MAX31865
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/max31865/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der MAX31865 ist ein benutzerfreundlicher Widerstand-zu-Digital-Wandler, der für Platin-Widerstandstemperaturdetektoren (RTDs) optimiert ist. Ein externer Widerstand stellt die Empfindlichkeit für den verwendeten RTD ein und ein Präzisions-Delta-Sigma-ADC wandelt das Verhältnis des RTD-Widerstands zum Referenzwiderstand in digitale Form um. Die Eingänge des MAX31865 sind gegen Überspannungsfehler bis Q45V geschützt. Eine programmierbare Erkennung von RTD- und Kabelunterbrechungen und Kurzschlüssen ist enthalten. MAX31865 wird in Industrieanlagen, Instrumenten und medizinischen Geräten verwendet.

LibDriver MAX31865 ist der Treiber mit vollem Funktionsumfang von MAX31865, der von LibDriver gestartet wurde. Er bietet Funktionen für das Lesen der PT-Temperatur im kontinuierlichen Modus, das Lesen der PT-Temperatur im Einzelmodus usw. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver MAX31865-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver MAX31865 SPI.

/test enthält den Testcode des LibDriver MAX31865-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver MAX31865-Beispielcode.

/doc enthält das LibDriver MAX31865-Offlinedokument.

/Datenblatt enthält MAX31865-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

#### example basic

```C
#include "driver_max31865_basic.h"

uint8_t res;
uint8_t i;
float temp;

res = max31865_basic_init(MAX31865_WIRE_4, MAX31865_RESISTOR_1000PT, 430.f);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    res = max31865_basic_read((float *)&temp);
    if (res != 0)
    {
        (void)max31865_basic_deinit();

        return 1;
    }
    max31865_interface_debug_print("max31865: temperature is %0.4fC.\n", temp);
    max31865_interface_delay_ms(1000);
    
    ...
    
}

...

(void)max31865_basic_deinit();

return 0;
```

#### example shot

```C
#include "driver_max31865_shot.h"

uint8_t res;
uint8_t i;
float temp;

res = max31865_shot_init(MAX31865_WIRE_4, MAX31865_RESISTOR_1000PT, 430.f);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    res = max31865_shot_read((float *)&temp);
    if (res != 0)
    {
        (void)max31865_shot_deinit();

        return 1;
    }
    max31865_interface_debug_print("max31865: temperature is %0.4fC.\n", temp);
    max31865_interface_delay_ms(1000);
    
    ...
    
}

...

(void)max31865_shot_deinit();

return 0;
```


### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/max31865/index.html](https://www.libdriver.com/docs/max31865/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.