# Git Notları — 30.08.2026

## Temel döngü
```bash
git status              # hangi daldayım, ne değişti
git add .               # sahneye koy
git commit -m "mesaj"   # mühürle
git push                # GitHub'a gönder
```

## Kurulum
```bash
git init                              # klasörü depoya çevir
git remote add origin <url>           # GitHub adresini tanıt
git remote -v                         # bağlı adresleri gör
git push -u origin main               # ilk push (-u bir kez)
```

## Branch
```bash
git branch                    # dalları listele (* = buradasın)
git checkout -b feature/x     # yeni dal aç ve geç
git checkout main             # dala geç
git branch -d feature/x       # dalı sil (merge edilmişse)
git push origin --delete x    # GitHub'daki dalı sil
```

İsimlendirme: `feature/...`, `fix/...`, `refactor/...`, `docs/...`

## Merge
```bash
git checkout main      # ÖNCE hedef dala geç
git merge feature/x    # SONRA kaynağı çek
```
- **Fast-forward**: main ilerlememişti, etiket kaydı
- **Merge commit**: iki hat da ilerlemişti, iki ebeveynli commit

## Merge conflict
Aynı satır iki dalda farklı değiştiyse olur.
Farklı satır/fonksiyon değiştiyse Git kendi halleder.

Dosyada görünen:
```
<<<<<<< HEAD
benim dalımdaki hali
=======
getirdiğim daldaki hali
>>>>>>> dal_adi
```

Çözüm:
1. `git status` → hangi dosya çakışmış
2. Dosyayı aç, istediğin hale getir, **işaretleri sil**
3. `git add <dosya>` → "çözdüm" demek
4. `git commit` → merge'ü tamamla

Vazgeçmek: `git merge --abort`

## .gitignore
```
build/
install/
log/
```
**Kritik:** Dosya zaten commit edildiyse .gitignore onu durdurmaz.
Önce takipten çıkar:
```bash
git rm -r --cached <klasor>    # --cached = diskten SİLME
```

## Uzak işlemler
- `git fetch` → indirir, dosyalara dokunmaz
- `git pull` → fetch + merge, hemen uygular
- `git push` → gönderir

**Kural:** İşe başlamadan önce `git pull`.

## Pull Request
Git'te yok, GitHub özelliği. "Dalımı ana dala alır mısın?" teklifi.
Kod incelemesi + yorum + otomatik test için.
Tek başına çalışırken gerekmez, ekipte şart.

## Yetki
Yerel branch açmak izin gerektirmez. İzin `push` anında sorulur.
Yetkin yoksa → fork + PR.

## Faydalı
```bash
git lg                        # alias: log --oneline --graph --all --decorate
git log --oneline --graph --all
git show <hash>               # bir commit'e bak
```

## Yaptığım hatalar
- Log temizliğini main yerine yanlış dalda yaptım → **her komuttan önce daldan emin ol**
- `-u` olmadan push → `no upstream branch` hatası
- `checkout` reddi: takip edilmeyen dosyalar üzerine yazılacaktı, Git durdu

## Bash notu
- `>` → dosyanın içini siler, yeniden yazar
- `>>` → sonuna ekler

## Öğrenilecekler
- [ ] `git pull` sırasında conflict
- [ ] `git stash` — yarım işi askıya alma
- [ ] `git revert` / `git reset` — geri alma
- [ ] Gerçek PR açıp merge etme
