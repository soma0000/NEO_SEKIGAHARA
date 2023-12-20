# NEO_SEKIGAHARA

開発環境
Unreal Engine5.1.1 Windows11

開発言語
C++

本作品はチーム制作であるため、下記に私の担当箇所を記載いたします。
またこだわった箇所には☆マークを付けます。l
その部分から見ていただけると幸いです。

担当箇所

\NEO_SEKIGAHARA\Source\DebugModule
☆中身全て

\NEO_SEKIGAHARA\Source\NEO\CharacterSystem

☆ActionAssistComponent.cpp
☆ActionAssistComponent.h
☆CharacterBase.cpp
☆CharacterBase.h

\NEO_SEKIGAHARA\Source\NEO\CharacterSystem\PlayerSystem

NEOPlayerController.cpp
NEOPlayerController.h
☆PlayerBase.cpp
☆PlayerBase.h
PlayerCharacter.cpp
PlayerCharacter.h

\NEO_SEKIGAHARA\Source\NEO\AnimationSystem

☆CharacterAnimInstance.cpp
☆CharacterAnimInstance.h
CharacterAnimNotify.cpp
CharacterAnimNotify.h
CharacterAnimNotifyState.cpp
CharacterAnimNotifyState.h

\NEO_SEKIGAHARA\Source\NEO\AnimationSystem\PlayerAnimation

☆PlayerAnimNotify.cpp
☆PlayerAnimNotify.h
PlayerAnimNotifyState.cpp
PlayerAnimNotifyState.h

\NEO_SEKIGAHARA\Source\NEO\WeaponSystem

Gun.cpp
Gun.h
Lance.cpp
Lance.h
PlayerBullet.cpp
PlayerBullet.h
Sword.cpp
Sword.h
WeaponBase.cpp
WeaponBase.h

担当箇所は以上となります。
-------------------------------------------------------------------------------------
ゲーム説明

XBOXコントローラーを想定しています。
十字キー：左右移動
RBボタン：武器を拾う
Aボタン：ジャンプ
Y,Bボタン：攻撃（連打でコンボがつながります）
いずれかのボタン：画面遷移
いずれかのボタン長押し：オープニングムービースキップ

キーボード
W：上に移動
A：左に移動
S：下に移動
D：右に移動
SPACE：ジャンプ
左クリック：攻撃
右クリック：攻撃

攻撃ボタン連打でコンボが可能でそれぞれのボタンを組み合わせてコンボが可能です。
また、プレイヤーは３回攻撃を食らうと武器を落としてしまいます。
武器を持っていない状態で攻撃を食らうと死んでしまうので、その前に落ちている武器を拾うとダメージがリセットされます。
敵が落とした武器も使うことができます。


