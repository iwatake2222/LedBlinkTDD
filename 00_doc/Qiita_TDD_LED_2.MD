TDDによるマイコンのLチカ開発(2)(完)

TDD 組み込み  デザインパターン 設計 マイコン

LチカをTDDに基づいて開発したときの、ソフト構造が作られていく過程を見ていきます。
前回(https://qiita.com/take-iwiw/items/396959d1d7dffee479f7 )の続きになります。


# LedBlinkを作る
## LedBlinkの責務とインターフェイスを考える
![01.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/d789a27f-a0e8-f7a7-868c-31b6a0fa471e.jpeg)

まず最初は、LedBlinkモジュールを作ろうとしました。LedBlinkには`void start(int interval)`というインターフェイスが必要で、全体的には上図のようなイメージになりました。

- LedBlinkの責務
	- 指定された間隔(ミリ秒)で、LEDを点滅させる

## LedBlinkをテストしようとする
TDDでは、 __テストFIRST__ が大事なので、まずLedBlinkのテストケースを書こうとします。
しかし、何をテストすべきかが分かりません。もう少し、LedBlinkの責務を分解してみます。

### LedBlinkをテストできるように設計を見直す
- LedBlinkの責務 (分解)
	- `start()`で、Timer0にコールバック登録する
	- コールバックが呼ばれたときに時間経過を調べて、指定間隔のたびに、LEDをON/OFFする

2つ目のLED ON/OFF確認は、LedBlinkが直接LEDをON/OFFしているというのが問題を難しくしてます。実際にLEDをON/OFFするのは別途LedDriverを用意して、そいつの責務にします。

- LedBlinkの責務 (最終)
	- `start()`で、Timer0にコールバック登録する
	- コールバックが呼ばれたときに時間経過を調べて、指定間隔のたびに、`LedDriver_turnOn()/LedDriver_turnOff()を呼ぶ

その結果、まずは以下のように、ソフト構造を見直しました。

![a.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/1afc94fb-ce78-ecf7-f24c-0ee97bb51e14.jpeg)


これでもまだ、LedBlinkはテストできません。LedBlinkをテストするためには、LedBlinkが依存しているLedDriverとTimer0モジュールが必要になります。さらに、それに紐づいて`avr/io.h`も必要になります。

テスト時には、両モジュールをFakeに置き換えて、Fake内で関数の呼ばれ方や状態を保持し、テストケースがそれを盗み見る、という方法でチェックできそうです。プロダクト用コードとFakeを切り替える方法はいろいろありますが、今回はInterfaceを使用しました。

プロダクト用関数とテスト用のFake関数で同じインターフェイス(関数の型)を持たせて、LedBlinkの呼び元で指定できるようにします(依存性の注入)。
その結果、ソフト構造を以下のようにしました。

![b.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/d4a0f200-b7f6-4437-e19b-1577ab1acfba.jpeg)


### LedBlinkのテストに必要なモジュール(ひとまずFake)を実装する
LedDriverFakeとTimer0Fakeを実装します。(プロダクト用の本物のLedDriverとTimer0はまだ実装しません。)

`interface`ですが色々と実装方法はあります。いっそ、C++にして __本当の__ interfaceにしてもいいと思います。Cでも模擬することが出来ます。あるいは、全関数の関数ポインタを突っ込むという方法でもできます。
今回は、関数ポインタを持つ構造体テーブルを使うことにしました。
この実装方法も、誰にどれくらい使われるか? マルチインスタンスは有り得るのか? などを考慮して決める必要があります。

コード全文は載せませんが、以下のような雰囲気です。

```c:LedDriver.h
typedef struct {
	void (*create)();
	void (*destroy)();
	void (*turnOn)();
	void (*turnOff)();
} ILedDriver;
extern ILedDriver LedDriver;
```

```c:LedDriverFake.h
extern ILedDriver LedDriverFake;
```

```c:LedDriverFake.c
ILedDriver LedDriverFake = {
	.create = LedDriverFake_create,
	.destroy = LedDriverFake_destroy,
	.turnOn = LedDriverFake_turnOn,
	.turnOff = LedDriverFake_turnOff,
};
```

また、Fakeモジュールには、結果を盗み見れるようなスパイ用関数を用意しました。
最後に、必要に応じて、Fakeモジュールのテストも行います。(これもユニットテストで、テストケースを書く)

## LedBlinkのテストをする
![c.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/3bb452e9-41d5-2b5d-cc04-a51abda27deb.jpeg)


ようやく、テスト対象モジュールであるLedBlinkのテストが出来るようになりました。テスト時の各モジュールの関連は上図のようになります。

初期化処理のテストを考えます。テストケース内では、必要な事前処理をして、`LedBlink_start()`を呼び、結果をFakeモジュールから盗み見て確認します。
最初は`LedBlink_start()`は未実装なので、テストは失敗します。

## テストが通るようにLedBlinkを実装します
テストが通るようにLedBlinkを実装します

## 他の処理についても、テスト作成 → 実装、する
例えば、コールバック時にちゃんとLEDを点滅するかどうかをテストします。
コールバックを発生させるために、Timer0Fakeに、割り込みを模擬する関数を追加します。
割り込みは1msec間隔で起きるので、1000回目を区切りにOFF->ONするかどうかをチェックしています。

このときのテストケースは以下のようになります。
__割り込みにからむ処理をちゃんとテストできたのはかなり凄いと感動しました。__

```c:LedBlinkTestCommon.c
TEST(LedBlinkTestCommon, blink)
{
	LedBlink_start(&LedDriverFake, &Timer0Fake, 1000);
	TEST_ASSERT_EQUAL(0, LedDriverFake_getStatus());
	for (int i = 0; i < 999; i++) {
		Timer0Fake_invokeIRQ();
	}
	TEST_ASSERT_EQUAL(0, LedDriverFake_getStatus());
	Timer0Fake_invokeIRQ();
	TEST_ASSERT_EQUAL(1, LedDriverFake_getStatus());
}
```



他にも、終了時の処理等、必要なケースを追加していきます。

## LedBlinkが完成した
これでLedBlinkは完成です。LedDriverとTimer0が期待通りに動けば、実機でLチカが動くはずです。
なお、これまで作成したテストはHostとTargetの両方で動かせるのでCommonのテストとしました。
(Hostだけのテストでもいいような気もしますが、特にHostだけにする必要もないのでCommonにします。)


# LedDriverを作る
流れとしてはLedBlinkと同じです。

## LedDriverの責務とインターフェイスを考える
LedDriverの責務とインターフェイスは、LedBlink作成時に既に決めていました。

- LedDriverの責務
	- `create()`でLED用GPIOを出力設定する
	- `turnOn()`でLED用GPIOをHighにする
	- `turnOff()`でLED用GPIOをLowにする
	- (LED用GPIOのポートは固定の決め打ちにする)


## LedDriverをテストしようとする
LedDriverは`avr/io.h`を使用しています。これは、ATMELから提供されているハードウェアレジスタにアクセスするためのマクロ群です。
例えば、`PORTB = 0xFF`は`(*(volatile uint8_t *)((0x05) + 0x20)) = 0xFF;`に展開されます。

### ハードウェアアクセスをどうするか?
ハードウェアへのアクセスですが、メモリマップドレジスタ方式においては、アドレスと値だけを定義した、下記のようなインターフェイスで一般化できます。

```c
void write(int address, int data);
int  read(int address);
```

今回もこの方式にすれば、CMockによる置き換えや、アクセス先のアドレスを適当な変数にして結果をダンプして確認する、といった手段でテストが可能になります。
しかし、大量のレジスタやビット制御のあるいまどきのマイコンに対して、そんなことはしたくありません。
せっかくマクロ(`avr/io.h`)が提供されているので、それを使うようにしたいです。

_Linuxでも`/dev/mem`をmemmapすればどんなデバイ制御もできますが、実際にはioctl経由での制御になると思います。これも、関数ポインタなんかじゃごまかせないです。_

### もしもこれが関数だったら
もしもハードウェアアクセス用のマクロが、「関数」として提供されていたら話は簡単になります。ハードウェアアクセス用関数はWrapper経由で使用するようにして、そのWrapperをFakeに置き換えることでテスト可能にできそうです。

例えば、STM32のHALであれば、ライブラリとして各種ハードウェアアクセスのためには関数(`HAL_GPIO_WritePin()`等)が提供されています。

![d.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/47ba37b8-00ca-c23d-5954-0d651d6efb77.jpeg)


### マクロにはマクロで対応するしかない
__もっといい方法があると思います。__
が、とりあえずマクロで定義されているものは、マクロで定義し返すという解決策しか思いつきませんでした。

```c:avr_wrapper.h
#ifndef UNIT_TEST_HOST

#include <avr/io.h>
#define AVR_WRAPPER_IO(X) X

#else

extern int Avr_wrapper_io_fake_val;
#define AVR_WRAPPER_IO(X) Avr_wrapper_io_fake_val
int Avr_wrapper_io_fake_getLastVal();
void Avr_wrapper_io_fake_setLastVal(int val);

#endif /* UNIT_TEST_HOST */
```

上記のようにマクロを定義して、自分のコード内からは`AVR_WRAPPER_IO()`経由でアクセスするようにします。で、ホスト環境でのユニットテストの時には変数にアクセスするようにします。その変数はavr_wrapper_fakeから盗み見れるようにします。

![e.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/6c0f7e63-1894-0719-072f-5ad0f261b178.jpeg)

## LedDriveのテストをする
最終的には、以下のような構成でLedDriverをテストします。

![f.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/e3239f7d-f11e-0b2f-547a-92d1b5df90e2.jpeg)


また、avr_wrapperを切り替えることで、Target上でもテストを流すことが出来ます。その際には期待値はLEDが実際に光ることなので目視での確認になります。(所定レジスタの値を読んで比較でもいいかもしれない。明るさセンサを持った自動テスト装置を作るということも可能)

![g.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/2b191af9-2a20-a945-6ab9-87948bc7c171.jpeg)


# Timer0を作る
力尽きたので、Timer0は普通に作った。

## あきらめた理由
`avr/io.h`にはレジスタアクセス用マクロに加えて、ビット位置指定の定義(OCIE0A)などがあり、Timer0ではそれらを使っています。
そのため、ホスト環境においても`avr/io.h`を取り込む必要があります。すると、`avr/io.h`がさらに取り込んでいる他のヘッダも取り込む必要があります。
ホスト環境のビルド時に、AVR toolchainのヘッダをincludeパスに含めればいいのですが、一般的なstdio.h等が競合してしまいました。
必要なヘッダファイルだけをコピーすれば解決できそうですが、あまりスマートじゃないので止めました。

# ここまでのソフト構造
ここまでの開発で、以下のような構造のソフトが出来上がりました。
実際に僕が実装したのはここまでになります。

![h.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/c8e0b137-ed1d-42e9-87c8-8e776c9b0fe4.jpeg)


## プロダクト用
プロダクト用では以下のような関連になります。

![i.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/c9f25d11-f0be-f0cb-5478-4cc687960f63.jpeg)


# 作ってはいないけど、設計だけ考えた
先ほどの`avr/io.h`に対する依存をもう少しスマートに解決できないかを考えました。考えただけなので、実装はしていません。

## GPIO Driverの導入
LedDriverがあまりにもそのままなので失念していましたが、通常はGPIO Driverを使いますよね。ということで、GPIO Driver経由で`avr/io.h`にアクセスします。
しかし、これはただ単に問題の先送りで、GPIO Driverのテストをどうするか? という問題にすり替わっただけです。

完璧な解決策ではないのですが、Host上でのテストはあきらめて、Target上でテストすればいいのかなと思いました。テストケース内で、所定のアドレスのレジスタの値をreadすることでチェックで出来そうです。

![j.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/9dbb2603-ad3a-2ca8-68ee-33eb7eb729f2.jpeg)


## Timer0の改善
Timer0の方はどうしましょう。テストしないという判断もありだとは思うのですが、登録されたコールバック関数の管理など、ロジックをチェックしたい所もあります。
悩んだのですが、ロジック部とハードウェアアクセス部に分けて、ロジック部だけテストするというのが現実的な落としどころかなと思いました。
Timer0_ioの関数の粒度ですが、あまり細かくすると大変な手間がかかります。（レジスタ一つ一つに対して分けると数十個の関数が必要になってしまいます。）
ですので、Timer0と同じくらいの粒度(init()など)でいいかなと思います。

![k.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/3b23a188-806d-2e2e-344b-08d0a59afeae.jpeg)


## テスト可能にする
それぞれテスト可能にするには、GPIODriverとTimer0_ioをFakeに置き換えればOKです。置き換え方法は何でもいいのですが、リンカで置き換えるのが一番楽かなと思います。(interfaceや関数ポインタでやるには手間がかかる割に、メリットが少ないと思います)

# 最終形態
ということで、Lチカ用の"ぼくのかんがえたさいきょうのソフト構造"は以下になります。

![l.jpg](https://qiita-image-store.s3.amazonaws.com/0/214268/03682edb-2a02-41ec-c253-bf7ab9a81d2c.jpeg)


_明らかにやりすぎだというのは分かっていますよ_

# おわりに
## メモ
テストケースを考える時には、明確な期待動作を定義することが必要になります。
__この作業ですが、テスト対象モジュール(関数)の責務を、抽象的なものから具体的な期待動作(コラボレータへの操作等)に変換するのがコツだと思いました。__
(例えば、「LEDをONにする」ではなく「GPIO Driverに対してturnOn発行する」など)
でも、あまり細かくしてしまうと、テストケース = 中身のコードとなってしまうので注意が必要そう。

## TDDによる恩恵
Lチカでやる意味はありませんが、以下のようなメリットがありました。

- LedBlinkはどんなLedDriverでも使えるようになった (インターフェイスが同じである限り)
	- Ledという名前を変えるべきでしたが、LED以外のどんなライトでも点滅できます
- LedBlinkはどんなTimerでも使えるようになった (インターフェイスが同じである限り)
	- これもTimer0の0が余計ですが、他のタイマやOSから提供されるようなタイマにも対応できます。
- テストをしながらの開発なので、品質も高いはず
	- でも、繰り返しになりますが、TDDの本当の狙いはテストによる品質向上よりも、良い設計にみちびくためのものだと思います

## 気になった点
- 開発時間
	- Lチカの実現だけなら30分くらいで出来るはずが、6時間くらいかかった (総ステップ数 = 971行)
- メンテの手間
	- モック(Fake)モジュールをたくさん作ったけど、メンテが大変そう
	- テストケースのメンテも大変そう
- 事前に全体をイメージできないと難しそう
- デバイス制御に対してテストFIRSTは難しそう
	- 新しいマイコンのペリやデバイスをいじるときって、色々と試行錯誤して試すと思います
	- 事前にテスト期待値を考えるのは難しそう。というかそれができれば仕事の半分は終わっているという。。。
	- でも
		- `start()`関数をよんだら、startビットが立ったね、みたいな簡単な確認になら使えそう
		- デバドラ内でもロジック部とハードウェア依存部があるので、そこを切れる設計につながるのはいいと思った
	- デバドラに関してはTDDではなく、ユニットテストだけを導入するのがコスパがいいと思った。
		1. デバドラが完成してそれなりに動いた後、レジスタダンプをしてそれを期待値として保存しておく。HW屋さんにもチェックしてもらう
		2. 定期的にその期待値と一致するかどうかの回帰テストをする
		3. みたいな感じ

## 感想
大変でした。
仕事で導入しようとすると、一時的にパフォーマンスが大幅に下がると思うので、上長やチームの理解が必要そうです。
長い目で見たらプラスになる(らしい)のですが、一度成功体験がないと厳しいんじゃないかと思います。自分のモチベーション的にも、説得材料的にも。

