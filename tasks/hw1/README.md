# HW1 – Labirent Kaşifi

> **Teslim:** 4 Ekim Pazar, 23:59 — Pull Request ile <br>
> **Kapsam:** class, constructor, public/private, kalıtım, `virtual`, `shared_ptr` / `unique_ptr`, `vector`, `switch`, dosya okuma

---

## ⚠️ BAŞLAMADAN ÖNCE — Kendi Branch'ini Aç

Bu ödevde **herkes kendi ismiyle bir branch açar** ve çalışmasını **sadece o branch'e** push'lar. `main`'e doğrudan commit **atılmaz**. İşin bitince **Pull Request (PR)** açarak birleştirme talebinde bulunursun.

**1. Repoyu klonla ve güncel hali al:**

```bash
git clone https://github.com/Enes004/robotaxi_27.git
cd robotaxi_27
git checkout main
git pull
```

**2. Kendi adına branch aç** (`hw1-<isim>` formatında):

```bash
git checkout -b hw1-ahmet        # kendi ismini yaz (hw1 yazdığından emin ol!)
git branch                        # kontrol: * hw1-ahmet'te olmalı
```

**3. Kendi klasörüne gir:**

```bash
cd tasks/hw1/ahmet
```

**4. Kodunu yaz, commit'le ve kendi branch'ine push'la:**

```bash
git add main.cpp map1.txt
git commit -m "hw1: temel tipler"
git push -u origin hw1-ahmet      # ilk sefer (-u: GitHub ile eşle)
```

**5. Pull Request aç (GitHub'da):**

- Repo sayfasında sarı kutuda **"Compare & pull request"** butonu çıkar
- `base: main ← compare: hw1-ahmet` yazdığını kontrol et
- Başlık: `HW1 – ahmet`
- Açıklama: `Refs #1` + 1–2 cümle açıklama
- **"Create pull request"**

> ❗ Yanlışlıkla `main`'de çalıştığını fark edersen hemen dur ve ekip liderine haber ver. Tek başına `git reset` **yapma**.

# Homework-1

## 1. Ne yapacaksın?

Terminalde çalışan küçük bir robot simülasyonu. Robot bir labirentte `S` noktasından başlar, **sağ el kuralı** ile yürüyerek `G` hedefine gitmeye çalışır. Her adımda bataryası azalır.

## 2. Dosya yapısı

Repo'da herkes **sadece kendi klasörüne** yazar.

```
robotaxi_27/
├── src/                    ← asıl proje (DOKUNMA)
├── tasks/
│   └── hw1/
│       ├── TASK.md         ← bu dosya (DOKUNMA)
│       ├── enes/           ← örnek: Enes'in klasörü
│       │   ├── main.cpp
│       │   └── map1.txt
│       ├── isim/          ← senin klasörün böyle görünecek
│       │   ├── main.cpp
│       │   └── map1.txt
│       └── ...
├── .gitignore              ← DOKUNMA
└── README.md               ← DOKUNMA
```

Klasörde sadece `main.cpp` ve `map1.txt` olacak. Task, aşağıdaki yönergeye göre adım adım `main.cpp` dosyasının içine yapılır.

## 3. Harita

Aşağıda `map1.txt` haritası bulunmaktadır:

```
##########
#S...#...#
#.##.#.#.#
#.#..#.#.#
#.#.##.#.#
#.#....#.#
#.######.#
#......#.#
#.####...#
#......#G#
##########
```

- `#` duvar, `.` boş, `S` başlangıç, `G` hedef manasına gelir.
- **x = sütun, y = satır**, sol üst köşe `(0,0)`.
- Kuzey: y azalır · Güney: y artar · Doğu: x artar · Batı: x azalır.

## 4. Adımlar

Her adımdan sonra derle, çalıştır, **commit at.**

### Adım 1 – Temel tipler (~30 dk)

```cpp
struct Position {
    int x{0};
    int y{0};
};

enum class Direction { North, East, South, West };

Direction turnLeft(Direction d) {
    // TODO: switch (d) kullanarak yönü sola çevirin (Örn: North -> West)
}

Direction turnRight(Direction d) {
    // TODO: switch (d) kullanarak yönü sağa çevirin (Örn: North -> East)
}

Position moveForward(Position p, Direction d) {
    // TODO: switch (d) kullanarak p noktasının d yönündeki komşu koordinatını döndürün
    // Hatırlatma: Kuzey -> y azalır, Güney -> y artar, Doğu -> x artar, Batı -> x azalır
}
```

Üç fonksiyonu da `switch` ile yaz.

> ❗Fonksiyonların sonuna varsayılan bir return Direction::North; veya return p; eklemeyi unutmayın, aksi takdirde derleyici uyarı verebilir.

**Kontrol:** `turnRight(Direction::West)` → `North` vermeli.

### Adım 2 – `Grid` sınıfı (~1 saat)

```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Grid {
public:
    bool load(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Harita acilamadi: " << path << "\n";
            return false;
        }

        cells_.clear();
        std::string line;
        while (std::getline(file, line)) {
            cells_.push_back(line);
        }

        // TODO: cells_ matrisini (satır ve sütunları) tarayın:
        // - 'S' karakterini bulunca start_ = {x, y} atayın.
        // - 'G' karakterini bulunca goal_ = {x, y} atayın.
        // - S veya G bulunamazsa hata mesajı yazdırıp false dönün.

        return true;
    }

    bool isFree(Position p) const {
        // TODO: Koordinat sınır kontrolü ve engel denetimi yapın:
        // 1. p.y < 0 veya p.y >= cells_.size() ise -> false
        // 2. p.x < 0 veya p.x >= cells_[p.y].size() ise -> false
        // 3. cells_[p.y][p.x] == '#' ise (duvar) -> false
        // Aksi takdirde hücre serbesttir -> true
        return false;
    }

    Position start() const { return start_; }
    Position goal()  const { return goal_; }

    void print(Position robotPos) const {
        // TODO: Haritayı ekrana yazdırın:
        // - cells_ matrisini satır satır ekrana basın.
        // - Eğer basılacak (x, y) konumu robotPos ile eşleşiyorsa o karakter yerine 'R' basın.
    }

private:
    std::vector<std::string> cells_;
    Position start_{0, 0};
    Position goal_{0, 0};
};
```

**Dikkat:** Hücreye erişim `cells_[y][x]` şeklindedir (önce satır!). `isFree` içinde sınır kontrolünü unutma, yoksa program çöker.

**Kontrol:** Haritayı yükle ve `print()` ile ekrana bas.

### Adım 3 – `Robot` taban sınıfı (~1.5 saat)

```cpp
#include <memory>

enum class Action { Forward, TurnLeft, TurnRight };

class Robot {
public:
    Robot(std::shared_ptr<const Grid> grid, double battery)
        : grid_(std::move(grid)), battery_(battery) {
        // TODO: Robotun başlangıç konumunu ayarlayın:
        // if (grid_) { pos_ = grid_->start(); }
    }

    virtual ~Robot() = default;

    void tick() {
        if (!grid_ || battery_ <= 0.0 || atGoal()) return;

        Action act = decide();

        // TODO: Seçilen eylemi uygulayın:
        // 1. Action::Forward ise:
        //    - Bataryayı 1.0 azaltın (battery_ -= 1.0).
        //    - moveForward(pos_, dir_) hücresi grid_->isFree() ile boş mu kontrol edin.
        //    - Boşsa pos_ değerini güncelleyin ve steps_++ artırın (duvarsa ilerlemez ama batarya yine de düşer).
        // 2. Action::TurnLeft ise:
        //    - Bataryayı 0.5 azaltın.
        //    - dir_ = turnLeft(dir_) ile yönü güncelleyin.
        // 3. Action::TurnRight ise:
        //    - Bataryayı 0.5 azaltın.
        //    - dir_ = turnRight(dir_) ile yönü güncelleyin.
    }

    bool atGoal() const {
        // TODO: Robot hedefe ulaştı mı kontrol edin:
        // return grid_ && pos_ == grid_->goal();
        return false;
    }

    double   battery()  const { return battery_; }
    int      steps()    const { return steps_; }
    Position position() const { return pos_; }

protected:
    virtual Action decide() = 0;       // Saf sanal fonksiyon: Türetilen robotlar dolduracak

    std::shared_ptr<const Grid> grid_; // Salt okunur paylaşılan harita
    Position  pos_{0, 0};
    Direction dir_{Direction::East};

private:
    double battery_{200.0};
    int    steps_{0};
};
```

`tick()` kuralları:

| Eylem | Batarya | Ne olur |
| --- | --- | --- |
| `Forward` | −1.0 | Öndeki hücre boşsa ilerler, `steps_` artar |
| `TurnLeft` / `TurnRight` | −0.5 | Sadece yön değişir |


### Adım 4 – `WallFollowerRobot` (~1 saat)

`Robot`'tan türet ve `decide()`'ı yaz. Sağ el kuralı: robot sağ elini duvara dayayıp yürür.

```cpp
class WallFollowerRobot : public Robot {
public:
    WallFollowerRobot(std::shared_ptr<const Grid> grid, double battery)
        : Robot(std::move(grid), battery) {}

protected:
    Action decide() override {
        // TODO: Sağ el kuralı karar algoritmasını uygulayın:
        // 1. Eğer justTurnedRight_ true ise:
        //    - justTurnedRight_ = false yapın ve Action::Forward döndürün (sağa döndük, o boşluğa giriyoruz).
        //
        // 2. Sağdaki komşu hücre boş mu kontrol edin:
        //    - Sağ yön: Direction rightDir = turnRight(dir_);
        //    - Sağ koordinat: Position rightPos = moveForward(pos_, rightDir);
        //    - Eğer grid_->isFree(rightPos) ise:
        //        justTurnedRight_ = true yapın ve Action::TurnRight döndürün.
        //
        // 3. Öndeki komşu hücre boş mu kontrol edin:
        //    - Ön koordinat: Position frontPos = moveForward(pos_, dir_);
        //    - Eğer grid_->isFree(frontPos) ise:
        //        Action::Forward döndürün.
        //
        // 4. Yukarıdakilerin hiçbiri değilse (sağ da ön de duvarsa):
        //    - Action::TurnLeft döndürün.

        return Action::Forward;
    }

private:
    bool justTurnedRight_{false};
};
```



### Adım 5 – `main()` (~30 dk)

```cpp
int main() {
    auto grid = std::make_shared<Grid>();
    if (!grid->load("map1.txt")) {
        return 1;
    }

    std::unique_ptr<Robot> robot = std::make_unique<WallFollowerRobot>(grid, 200.0);

    for (int t = 0; t < 500; ++t) {
        if (robot->atGoal() || robot->battery() <= 0.0) {
            break;
        }
        robot->tick();
    }

    // TODO: Simülasyon sonucunu terminale yazdırın:
    // - robot->atGoal() true ise "Hedefe ulasildi!", false ise "Hedefe ulasilamadi!"
    // - Toplam atılan adım: robot->steps()
    // - Kalan batarya: robot->battery()

    return 0;
}
```

Beklenen: Robot map1'de hedefe ulaşmalı.

## 5. Derleme

```bash
cd tasks/hw1/<isim>                              # kendi klasörüne gir
g++ -std=c++17 -Wall -Wextra main.cpp -o hw1  # derle
./hw1                                         # çalıştır
```

## 6. Ek

### 6.1 Branch nedir, neden kullanıyoruz?

`main` → projenin **ortak ve temiz** hali. Kimse doğrudan `main`'e kod atamaz (kilitli).

**Branch** → `main`'in senin için açılmış bir kopyası. Kendi branch'inde istediğin kadar deneme yaparsın, `main` bozulmaz. İşin bitince **Pull Request (PR)** ile "kodumu `main`'e alır mısınız?" diye istek atarsın. Kontrol edilir, onaylanınca **merge** edilir, yani `main`'e eklenir.

```
main:          ●────────────────────────────●  (merge sonrası senin kodun da burada)
                \                          /
hw1-ahmet:       ●──●──●──●  → PR → review ─
                 commit'ler
```

- **commit** → yaptığın değişikliğin fotoğrafı (bilgisayarında durur)
- **push** → commit'leri GitHub'a gönder
- **PR** → "branch'imi main'e almak ister misiniz?" isteği
- **merge** → onaylanan kodun main'e eklenmesi

### 6.2 Kurallar

- Sadece `tasks/hw1/<isim>/` içine yaz, başka hiçbir dosyaya dokunma.
- `git add .` **kullanma**, her zaman kendi klasörünü ekle.
- Yapay zekâ kullanabilirsin ama yazdığın her satırı açıklayabilmelisin. Kodundan rastgele bir fonksiyonu anlatman istenebilir.
