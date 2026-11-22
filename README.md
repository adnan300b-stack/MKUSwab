# MKUSwap — إضافة تبديل شخصيات لـ Mortal Kombat Unchained (PPSSPP)

إضافة (Plugin) مبنية على أساس [MKUHook](https://github.com/ermaccer/MKUHook) لكن بدون الميزات الإضافية،
وظيفتها الوحيدة: **تبديل مجسم وضربات شخصية بأخرى مع بقاء المهارات السحرية للشخصية المختارة.**

## المتطلبات

- نسخة اللعبة **ULUS10102** (نسخة أمريكية من Mortal Kombat: Unchained)
- محاكي **PPSSPP 1.11.3 أو أحدث**
- عطل خيار **Fast Memory** في إعدادات PPSSPP (System ← uncheck "Fast Memory") وإلا قد تتجمد اللعبة

## التثبيت

```
PSP/
└── PLUGINS/
    └── mkuswap/
        ├── mkuswap.prx      ← ناتج البناء
        ├── Settings.ini     ← ملف الإعدادات
        └── logs.log         ← يُنشأ تلقائياً
```

1. انسخ مجلد `mkuswap` إلى `PSP/PLUGINS/` (في ويندوز غالباً في Documents/PPSSPP، وعلى أندرويد في جذر بطاقة الذاكرة)
2. فعّل الإضافة من PPSSPP: `Settings → Tools → Plugins` ثم فعّل **mkuswap.prx**
3. أعد تشغيل اللعبة

## طريقة الاستخدام

افتح `Settings.ini` وأضف أسطراً بالصيغة:

```
chang(الشخصية_الظاهرة->الشخصية_المختارة)
```

مثال — طلبك بالضبط:

```
chang(SCORPION->SUBZERO)
```

عند اختيار **Sub-Zero**:
- ✅ المجسم: Scorpion
- ✅ الضربات الأساسية والأنميشن: Scorpion
- ✅ المهارات السحرية (كرة الثلج…): Sub-Zero (لا تتغير)
- ✅ الاسم في شريط الصحة: SCORPION

الأسماء غير حساسة لحالة الأحرف وتتجاهل المسافات والشرطات (`SOBZERO` = `Sub-Zero`).
الحد الأقصى 32 سطر تبديل. الأسطر التي تبدأ بـ `;` أو `#` تعليقات.

## قائمة الأسماء المتاحة

`SCORPION, BARAKA, NOOB, SUBZERO, MILEENA, NIGHTWOLF, ERMAC, ASHRAH, SINDEL, LIMEI, BORAICHO, HOTARU, KENSHI, SMOKE, HAVIK, TANYA, LIUKANG, GHOST, KIRA, KABAL, KOBRA, JADE, DAIROU, RAIDEN, DARRIUS, SHUJINKO, YOUNGSHUJINKO, NOOBSMOKE, MONSTER, ONAGA, GORO, SHAOKAHN, FROST, KITANA, JAX, BLAZE` ونسخ MKDA من بعض الشخصيات + `MOKAP, KANO` (هذان يحتاجان ملفات `mkda_mokap.*` / `mkda_kano.*` داخل مجلد اللعبة كما في MKUHook).

## ملف السجل logs.log

كل ما يفعله الكود يسجّل هنا: القراءة، التبديلات المُسجّلة، كل عملية تحميل مجسم/سكربت، والأخطاء (سطر غير صالح، اسم غير معروف…). إذا لم يعمل التبديل افتح هذا الملف أولاً.

## البناء (Build)

يلزم [PSPSDK](https://github.com/pspdev/pspdev):

```bash
cd MKUSwap
make
```

سيُنتج `../bin/mkuswap.prx`، أو استخدم ملف الحل `MKUSwap.sln` مع Visual Studio.

## كيف يعمل؟ (للمطورين)

اللعبة تحمّل كل مقاتل على 3 مراحل، والإضافة تعترضها:

| المرحلة | الدالة المُعترضة | السلوك |
|---|---|---|
| مجسم + أنميشن + أنماط القتال (`load_ssf`) | `custom_load_ssf` / `_alt` / `_async` | تحمّل جدول الشخصية **الظاهرة** ← المجسم والضربات تتغير |
| سكربت الحركات `.mko` | `custom_load_script_data` / `_async` | يبقى سكربت الشخصية **المختارة** ← المهارات السحرية لا تتغير |
| أسماء اللاعبين | `player1_name` / `player2_name` / `set_win_name` | يعرض اسم الشخصية الظاهرة |

> ملاحظة صدق: سكربت المهارات يشير إلى أنميشن بأسماء محددة. معظم الحركات العامة (مشي، جري، ارتطام) تعمل، لكن إذا كانت حركة سحرية تشير لاسم أنميشن غير موجود في ملفات الشخصية الظاهرة فقد لا تظهر بشكل مثالي — هذا حد طبيعي في محرك اللعبة وليس خطأ في الكود. لو أردت تبديل الحركات بالكامل (سكربت + أنميشن) غيّر `picked` إلى `SwapMgr_GetVisual(picked)` داخل `custom_load_script_data` في `code/mkuplrinfo.c`.

## الميزات التي أزيلت من MKUHook

شاشة اختيار الـ NPCs، المراحل الإضافية، تبديل الألوان البديلة، معدّل عمر Shujinko، معدّل الـ Puzzle، ومكتبة minIni (التحليل هنا مخصص لصيغة `chang()`).
