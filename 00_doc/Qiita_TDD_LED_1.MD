TDDによるマイコンのLチカ開発(1)

TDD 組み込み  デザインパターン テスト 設計 

# はじめに
TDDに基づいてAVRマイコンでLチカを開発してみました。
『テスト駆動開発による組み込みプログラミング』を読んで分かった気になっていたのですが([こちらの記事参照](https://qiita.com/take-iwiw/items/03e5c856a906fc18cdb5))、実際にやると色々と大変でした。
AVRマイコンを題材にしていますが、一般の組み込み開発でも参考になると思います。また、マイコン開発にはあえてIDE(Atmel Studio)を使いました。これは、ほとんどのマイコンではIDEが提供されており、開発ではそれを使うケースが多いと考えたためです。他のマイコン用IDEでも同様の手順でやれると思います。自前のMakefile(or CMake等)ならもっと楽だと思います。

プロジェクト一式はここ
https://github.com/take-iwiw/LedBlinkTDD

## やったこと
- AVRによるLチカをTDDで開発し、設計をブラッシュアップする
- 各モジュールをユニットテスト可能にする
- ユニットテストは、Host, Targetの両方でできるようにする
	- 実際の現場でやるかは別として、とりあえずできることを確認したかった

## TDD導入で何が変わったか
開発の流れは次回以降に説明しますが、最初に、Lチカを実現するソフト構造がどう変わったかを示します。

まず最初は、下記のような構造を頭の中に思い描いてました(実際にコーディングはしていないし、してはいけません)。よくあるLチカですね。時間取得のためには、delay関数じゃなくてハードウェアタイマを使いました。
(`avr/io.h`はtoolchainによって提供される、ハードウェアアクセスマクロ群です)

![image.png](https://qiita-image-store.s3.amazonaws.com/0/214268/34eccbaa-2aa1-af81-ddf8-6b0d6e4106e1.png)

↑の設計が、TDDに基づいて開発していく中でこうなりました。かなり大げさになってしまいました。。。
TDDがただのテストではなく、設計(開発)のためのツールだというのがよくわかりますね。

![image.png](https://qiita-image-store.s3.amazonaws.com/0/214268/c037a725-12e0-ee10-1e18-c6680808955d.png)

一応、依存性の少ないSOLIDな設計になったのかな。。。
(もちろん、このソフト構造はLチカに対してはやりすぎですが)

# 開発環境の用意
## 開発環境
- Target
	- Device: ATmega328P (arduinoではなく素のAVR)
	- IDE: Atmel Studio 7.0
- Host
	- Editor: VSCode (can be any)
	- System: gcc (MinGW on Windows10 (can be any))
	- Build tool: CMake
- Unit Test Framework
	- Unity (https://github.com/ThrowTheSwitch/Unity)

## 使用するテストフレームワークの選定
Target上でも動かしてみたかったので、軽量なUnityを採用しました。
オーバーヘッドは約プラス4KByteでした。(これでも、でかい。。。)

### Targetマイコン上でテストする必要があるのか?
コストと効果のトレードオフですが、場合によってはやる価値はあると思います。

- ハードウェアやレジスタアクセスを絡めた、Targetマイコン上ならではのテストが出来る
	- 人の手による操作や、目視による確認が必要になるかもしれないが、それでも嬉しいケースはあると思います
	- 例えば、「異常発生時に警告LEDを光らせる」ということを確認するときに、LEDのチェックは人の目でやる必要がありますが、異常発生させるのはテストケースを呼ぶだけでできるので、通常のテストに比べてかなり楽になります
	- ハードウェアの受け入れ時や、週一くらいでやる回帰テストに向いていそうに感じました
- ロジックだけのモジュールでも、アーキテクチャの違いによる問題をテストできる
	- 特に今回使用したAVRだと8-bitなので、オーバーフロー問題などが実機だと発生する可能性があります。
	- (ちゃんと定義にuint8_t系を使えば、HOST上でもある程度は不具合検知できるとは思います)

また、ビルド、書き込みが短時間で気軽にできるようでしたら、全テストをターゲット上でやるというのもありかもしれません。


# 開発環境の用意
Atmel Studioを例に説明しますが、他のマイコン、他のIDE(または、自前のMakefile)でも同じようにできると思います。
Atmel Studioには以下のような特徴があったので、そこに注意する必要がありました。(他のIDEも同じだと思います)

- プロジェクトに含まれているソースファイルは、自動的にビルド対象になる (手動で有効/無効にはできる)
- プロジェクトに必要なソースファイル/フォルダは、IDE上で追加登録する必要がある (エクスプローラ上で作成するだけじゃだめ)

ここに全コードは載せないので、必要に応じて以下を参照してください。(本実装前の環境構築直後の状態です)
https://github.com/take-iwiw/LedBlinkTDD/tree/b6e2e8375c73f5656b73474a835090d5a937e386

## マイコンIDE上でプロジェクトを作成する
プロジェクトを作成して、以下のようなディレクトリ構造をIDE内で作っておきます。

```txt:プロジェクト構造(IDE上)
Include/
Mock/
   - Common/
   - Target/
Src/
   - main.c
Test/
   - Common/
   - Target/
Unity/
```

CommonはHostとTargetのテストの両方で必要なファイル、TargetはTarget上のテストにのみ必要なファイルです。Host上だけ、またはTarget上だけでしかテストしない場合は分けないでいいと思います。
Host上のテストにのみ必要なファイル/フォルダはここには登録しません。

## UARTドライバを作る
Unityの結果出力のために必要になります。`putchar()`相当の関数を1つ用意すればいいです。LCD出力でもいいですが、今回はUARTを使います。
簡単なLチカの開発をしているのに、より難しいUARTドライバが必要になるのは本末転倒な気がしますが、ご容赦ください。Lチカはあくまで練習のための例題なので。

今回は、以下のような関数を事前に作りました。

```c:Uart.h
void Uart0_send(uint8_t data);
```
## Unityをポーティングする
### ソースコードのコピー
Unity(https://github.com/ThrowTheSwitch/Unity )とCMock(https://github.com/ThrowTheSwitch/CMock )から以下のソースコードを、プロジェクト内のUnityフォルダに追加します。(IDE上でAdd Existing Itemしてください)。
`cmock.c, cmock.h, cmock_internals.h, unity.c, unity.h, unity_fixture.c, unity_fixture.h, unity_fixture_internals.h, unity_fixture_malloc_overrides.h, unity_internals.h`

(本当は、ソースコードコピーじゃなくて、git cloneして参照するようにした方がいいと思うのですが、横着しています)

### メモリ使用量を抑える
デフォルトだと、CMockは32KByteのヒープメモリを使用します。これだと入らないので、サイズを小さくします。(そもそもMockを使わない場合は、ソースコードから除外してもOKです)

```c:cmock_internals.h
#ifndef CMOCK_MEM_SIZE
//#define CMOCK_MEM_SIZE (32768)
#define CMOCK_MEM_SIZE (1024)
#endif
```

コンパイルオプションに`-DCMOCK_MEM_SIZE=1024`を付けるのでもいいと思います。

### 結果出力用関数を置き換える
デフォルトだと、Unityは結果を出力するために`putchar()`関数を使います。これを、先ほど用意した`Uart0_send()`を使うように変更します。

コンパイルオプションに以下を追加します。(Atmel Studioなら、Solution Explorerでプロジェクト名を右クリック -> Properties -> AVR/GNU C Compiler -> Symbols -> Defined synbols (-D)に追加)

- `UNITY_OUTPUT_CHAR=outputChar`
- `UNITY_OUTPUT_CHAR_HEADER_DECLARATION=(*outputChar)(char)`

続いて、ソースコード内の適当な位置に、`void (*outputChar)(char) = Uart0_send;`を追加します。僕は、Target用テストランナー(後述) と同じところに置きました。


### テストランナーを用意する
Target用のテストランナーを`Test/Target/`に、TargetとHost共用のテストランナーを`Test/Common/`に、用意します。ターゲット用テストランナーからcommonを呼ぶようにしています。


```c:AllTestTarget.c
#include "Unity/unity_fixture.h"
#include "Driver/Uart0/Uart0.h"

static void AllTestTarget(void)
{
	RUN_TEST_GROUP(TestXXX);
}

void runTestTarget(int argc, const char * argv[])
{
	extern void AllTestCommon(void);
	UnityMain(argc, argv, AllTestCommon);

	UnityMain(argc, argv, AllTestTarget);
}

void (*outputChar)(char) = Uart0_send;
```

### main関数からテストランナーを呼ぶ
最後に、mainから呼んで終わりです。下記は横着して`#if 1`で切り替えていますが、コンパイルスイッチから切り替えできるようにするといいと思います。

```c:main.c
int main(void)
{
	Uart0_defaultInit();

#if 1
	extern void runTestTarget(int argc, const char * argv[]);
	runTestTarget(0, NULL);
#endif

	while(1);
}
```

## Host用テスト環境をIDE外で作成する
Host用テストでのみ使用するソースコードは、IDE外(例えば、Windowsエクスプローラ)で追加作成します。これは、IDE上で追加するとビルド対象になってしまうためです。(手動で除外することもできますが、面倒なのでこうします)

```txt:プロジェクト構造(全ファイル)
Include/
Mock/
   - Common/
   - Target/
   - Host/      <- 追加
Src/
   - main.c
Test/
   - Common/
       - AllTestCommon.c (IDE上で追加済み)
   - Target/
       - AllTestTarget.c (IDE上で追加済み)
   - Host/      <- 追加
       - AllTestHost.c  <- 追加
Unity/
CMakeLists.txt  <- 追加
```

`AllTestHost.c`はmain関数を含み、テストランナーを呼び出します。
`CMakeLists.txt`では、`Src/main.c`をビルド対象外にするようにしておきます。`Unity/`内の全ファイルと、必要なファイルをビルドするように設定します。(横着して一つのCMakeLists.txtしか用意していませんが、ちゃんとディレクトリ毎にCMakeLists.txtを用意した方がいいです)

# 実行してみる
## Target
通常通り、ビルド、書き込み、をしたら動くと思います。結果はUARTから出力されます。

## Host
Host上の適当なターミナル(MinGW等)で以下コマンドで実行できます。

```
mkdir BuildHost && cd BuildHost
cmake .. -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=DEBUG
make
./bin/LedBlinkTestHost.exe  -v
```
