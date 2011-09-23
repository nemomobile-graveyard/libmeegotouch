/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "pt_mlocale.h"

#define VERBOSE_OUTPUT

void Pt_MLocale::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "" };
    qap = new QCoreApplication(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QProcess process;
#ifdef HAVE_ICU
    process.start("sh -c \"dpkg -s libicu44 | grep Version | perl -pe 's/^Version:[[:space:]]*([^[[:space:]]+)$/$1/g'\"");
    if (!process.waitForFinished()) {
        qDebug() << "cannot run process to check libicu44 package version , exiting ...";
        exit(1);
    }
    icuPackageVersion = process.readAllStandardOutput();
    icuPackageVersion.replace('\n', "");
    qDebug() << "libicu44 package version is:" << icuPackageVersion;
#endif
}

void Pt_MLocale::cleanupTestCase()
{
    delete qap;
}

void Pt_MLocale::init()
{
}

void Pt_MLocale::cleanup()
{
}

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkSingleLanguageEndonym()
{
    QString language("de_CH");   // only this should matter
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA");     // should not matter
    QString lcNumeric("en_US");  // should not matter
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);

    QString result("Schweizer Hochdeutsch");
    QCOMPARE(locale.languageEndonym(), result);

    QBENCHMARK {
        locale.languageEndonym();
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkAllLanguageEndonym()
{
    QList<QString> localeNames;
    int numberOfAvailableLocales = uloc_countAvailable();
    for (int i = 0; i < numberOfAvailableLocales; ++i)
        localeNames << QString::fromUtf8(uloc_getAvailable(i));

    QBENCHMARK {
        foreach (QString localeName, localeNames) {
            MLocale locale(localeName);
#if 0
            QTextStream debugStream(stdout);
            debugStream.setCodec("UTF-8");
            debugStream << locale.name() << " " << locale.languageEndonym() << "\n";
#endif
            locale.languageEndonym();
        }
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkSingleCountryEndonym()
{
    QString language("de_CH");   // only this should matter
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA");     // should not matter
    QString lcNumeric("en_US");  // should not matter
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);

    QString result("Schweiz");
    QCOMPARE(locale.countryEndonym(), result);

    QBENCHMARK {
        locale.countryEndonym();
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkAllCountryEndonym()
{
    QList<QString> localeNames;
    int numberOfAvailableLocales = uloc_countAvailable();
    for (int i = 0; i < numberOfAvailableLocales; ++i)
        localeNames << QString::fromUtf8(uloc_getAvailable(i));

    QBENCHMARK {
        foreach (QString localeName, localeNames) {
            MLocale locale(localeName);
#if 0
            QTextStream debugStream(stdout);
            debugStream.setCodec("UTF-8");
            debugStream << locale.name() << " " << locale.countryEndonym() << "\n";
#endif
            locale.countryEndonym();
        }
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkFormatNumberQLongLongArabic()
{
    QString localeName("ar");
    QString localeNameLcNumeric("ar_EG@numbers=arab");
    qlonglong number = qlonglong(-1542678073);
    QString formatted("‪١٬٥٤٢٬٦٧٨٬٠٧٣-‬");
    MLocale locale(localeName);
    locale.setCategoryLocale(MLocale::MLcNumeric, localeNameLcNumeric);
#if 0
    QTextStream stream(stdout);
    stream.setCodec("UTF-8");
    stream << " localeName=" << localeName
           << "\n localeNameLcNumeric=" << localeNameLcNumeric
           << "\n number=" << number
           << "\n locale.formatNumber(number)=" <<  locale.formatNumber(number)
           << "\n formatted=" << formatted;
    stream.flush();
#endif
    QCOMPARE(locale.formatNumber(number), formatted);
    QBENCHMARK {
        locale.formatNumber(number);
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkFormatNumberQLongLongWestern()
{
    QString localeName("de_CH");
    QString localeNameLcNumeric("de_CH");
    qlonglong number = qlonglong(-1542678073);
    QString formatted("-1'542'678'073");
    MLocale locale(localeName);
    locale.setCategoryLocale(MLocale::MLcNumeric, localeNameLcNumeric);
#if 0
    QTextStream stream(stdout);
    stream.setCodec("UTF-8");
    stream << " localeName=" << localeName
           << "\n localeNameLcNumeric=" << localeNameLcNumeric
           << "\n number=" << number
           << "\n locale.formatNumber(number)=" <<  locale.formatNumber(number)
           << "\n formatted=" << formatted;
    stream.flush();
#endif
    QCOMPARE(locale.formatNumber(number), formatted);
    QBENCHMARK {
        locale.formatNumber(number);
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkFormatNumberDoubleArabic()
{
    QString localeName("ar");
    QString localeNameLcNumeric("ar_EG@numbers=arab");
    double number = double(-1234567.1234567);
    QString formatted("‪١٬٢٣٤٬٥٦٧٫١٢٣-‬");
    MLocale locale(localeName);
    locale.setCategoryLocale(MLocale::MLcNumeric, localeNameLcNumeric);
#if 0
    QTextStream stream(stdout);
    stream.setCodec("UTF-8");
    stream << " localeName=" << localeName
           << "\n localeNameLcNumeric=" << localeNameLcNumeric
           << "\n number=" << number
           << "\n locale.formatNumber(number)=" <<  locale.formatNumber(number)
           << "\n formatted=" << formatted;
    stream.flush();
#endif
    QCOMPARE(locale.formatNumber(number), formatted);
    QBENCHMARK {
        locale.formatNumber(number);
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkFormatNumberDoubleWestern()
{
    QString localeName("de_CH");
    QString localeNameLcNumeric("de_CH");
    double number = double(-1234567.1234567);
    QString formatted("-1'234'567.123");
    MLocale locale(localeName);
    locale.setCategoryLocale(MLocale::MLcNumeric, localeNameLcNumeric);
#if 0
    QTextStream stream(stdout);
    stream.setCodec("UTF-8");
    stream << " localeName=" << localeName
           << "\n localeNameLcNumeric=" << localeNameLcNumeric
           << "\n number=" << number
           << "\n locale.formatNumber(number)=" <<  locale.formatNumber(number)
           << "\n formatted=" << formatted;
    stream.flush();
#endif
    QCOMPARE(locale.formatNumber(number), formatted);
    QBENCHMARK {
        locale.formatNumber(number);
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkChineseSorting()
{
    MLocale localeZh("zh_CN@collation=pinyin");
    MLocale localeEn("en_US");
    QString str("居 于 中 国 疆 域 内 的 各 个 民 族 统 称 为 中 华 民 族 亦 称 中 国 人 或 华 人 华 族 移 居 境 外 的 华 人 多 被 称 为 海 外 华 人 汉 族 是 中 国 人 口 最 多 地 域 分 布 最 广 的 民 族 在 少 数 民 族 当 中 人 过 年 口 查 有 满 苗 吾 彝 家 古 藏 9 民 通 汉 少 民 地 往 使 各 族 己 语 中 的 统 术 式 国 戏 书 和 画 传 娱 活 有 棋 棋 麻 等 国 格 术 术 世 名 酒 和 子 为 国 特 饮 文 春 中 新 元 清 端 七 中 重 冬 等 传 节 中 传 上 一 儒 思 国 中 人 强 的 庭 念 此 代 家 的 氏 于 名 前 国 还 视 育 对 才 选 春 时 孔 有 无 因 施 开 办 塾 养 才 朝 采 察 推 政 官 隋 起 行 举 平 中 拔 才 古 来 文 科 技 等 面 成 浩 烟 明 以 西 的 代 技 民 政 思 传 中 中 歷 重 編 历 历 历 都 有 官 此 存 十 详 的 史 料 二 四 资 通 等 今 4 0 多 前 中 地 为 心 始 现 落 织 而 国 和 代 历 多 演 和 代 迭 续 间 长 朝 有 商 汉 宋 和 等 朝 代 出 过 平 世 汉 文 之 和 武 兴 朝 观 治 开 盛 明 万 中 清 康 盛 等 學 究 示 國 代 更 常 氣 與 食 產 影 会 致 在 乱 分 时 中 王 历 上 断 北 游 民 交 征 如 奴 厥 卑 古 女 满 等 中 晋 代 经 五 乱 而 古 女 甚 入 中 改 了 国 世 的 史 展 程 多 族 入 华 族 大 庭 亥 命 中 的 王 体 1 1 年 出 史 台 而 之 是 和 体 中 民 後 国 内 時 民 府 臺 形 中 民 實 統 台 地 與 生 權 华 民 和 實 統 大 地 分 的 面 居 于 中 国 疆 域 内 的 各 个 民 族 统 称 为 中 华 民 族 亦 称 中 国 人 或 华 人 华 族 移 居 境 外 的 华 人 多 被 称 为 海 外 华 人 汉 族 是 中 国 人 口 最 多 地 域 分 布 最 广 的 民 族 在 少 数 民 族 当 中 人 过 年 口 查 有 满 苗 吾 彝 家 古 藏 9 民 通 汉 少 民 地 往 使 各 族 己 语 中 的 统 术 式 国 戏 书 和 画 传 娱 活 有 棋 棋 麻 等 国 格 术 术 世 名 酒 和 子 为 国 特 饮 文 春 中 新 元 清 端 七 中 重 冬 等 传 节 中 传 上 一 儒 思 国 中 人 强 的 庭 念 此 代 家 的 氏 于 名 前 国 还 视 育 对 才 选 春 时 孔 有 无 因 施 开 办 塾 养 才 朝 采 察 推 政 官 隋 起 行 举 平 中 拔 才 古 来 文 科 技 等 面 成 浩 烟 明 以 西 的 代 技 民 政 思 传 中 中 歷 重 編 历 历 历 都 有 官 此 存 十 详 的 史 料 二 四 资 通 等 今 4 0 多 前 中 地 为 心 始 现 落 织 而 国 和 代 历 多 演 和 代 迭 续 间 长 朝 有 商 汉 宋 和 等 朝 代 出 过 平 世 汉 文 之 和 武 兴 朝 观 治 开 盛 明 万 中 清 康 盛 等 學 究 示 國 代 更 常 氣 與 食 產 影 会 致 在 乱 分 时 中 王 历 上 断 北 游 民 交 征 如 奴 厥 卑 古 女 满 等 中 晋 代 经 五 乱 而 古 女 甚 入 中 改 了 国 世 的 史 展 程 多 族 入 华 族 大 庭 亥 命 中 的 王 体 1 1 年 出 史 台 而 之 是 和 体 中 民 後 国 内 時 民 府 臺 形 中 民 實 統 台 地 與 生 權 华 民 和 實 統 大 地 分 的 面");
    QStringList list = str.split(QLatin1String(" "),QString::SkipEmptyParts);
    QBENCHMARK {
        qSort (list.begin(), list.end(), localeEn.collator());
        qSort (list.begin(), list.end(), localeZh.collator());
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkCollatorStrengthSwitching()
{
    MLocale localeZh("zh_CN@collation=pinyin");
    MLocale localeDa("da_DK");
    MCollator collatorZh = localeZh.collator();
    MCollator collatorDa = localeDa.collator();
    QBENCHMARK {
        collatorDa.setStrength(MLocale::CollatorStrengthPrimary);
        collatorDa.setStrength(MLocale::CollatorStrengthQuaternary);
    }
}
#endif
QTEST_APPLESS_MAIN(Pt_MLocale);
