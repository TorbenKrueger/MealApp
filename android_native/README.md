# Android Native Port (Skeleton)

Dieses Verzeichnis enthält ein grundlegendes Android-Studio-Projekt als Ausgangspunkt für die Portierung der QML/Qt-Anwendung.

* `MainActivity.kt` entspricht grob dem `ApplicationWindow` aus `main.qml` und hostet die Navigation.
* Die einzelnen Fragmente (`HomePageFragment`, `WeekViewFragment` usw.) sind Platzhalter für die jeweiligen QML-Komponenten.
* Das Navigationsdiagramm `nav_graph.xml` spiegelt die Wechsel zwischen den Seiten wider, wie sie im QML `SwipeView` realisiert sind.

Die eigentliche Logik der C++-Klassen und QML-Skripte wurde hier **nicht** übersetzt. Stattdessen dient das Projekt als Startpunkt für eine manuelle Migration nach Kotlin. Weitere Anbindung der Datenmodelle muss separat erfolgen (z.B. per LiveData oder Kotlin Coroutines).
