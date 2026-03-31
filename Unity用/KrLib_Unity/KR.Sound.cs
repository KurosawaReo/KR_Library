/*
   - KR.Sound - (Unity)
   ver.2026/03/21
*/
using System;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// サウンド用の追加機能.
/// </summary>
namespace KR.Unity.Sound
{
    /// <summary>
    /// サウンドデータ.
    /// </summary>
    [Serializable]
    public struct SoundData
    {
        [SerializeField] string    name;  //登録名.
        [SerializeField] AudioClip audio; //コンポーネント.
        //get.
        public string    Name  { get => name; }
        public AudioClip Audio { get => audio; }
    }

    /// <summary>
    /// SoundManager用機能[継承想定]
    /// </summary>
    public class SoundMngKR : MonoBehaviour
    {
        public static SoundMngKR Inst; //実体を入れる用.

        [Header("- SoundMngKR -")]
        [Space(4)]
        [SerializeField] AudioSource     audioSource;
        [Space(4)]
        [SerializeField] List<SoundData> bgmData; //BGMデータ配列.
        [SerializeField] List<SoundData> seData;  //SE データ配列.

        Dictionary<string, AudioClip> bgmClips = new Dictionary<string, AudioClip>(); //BGM保存用.
        Dictionary<string, AudioClip> seClips  = new Dictionary<string, AudioClip>(); //SE 保存用.

        /// <summary>
        /// SoundMngKRの初期化.
        /// </summary>
        public void InitSoundMngKR()
        {
            if (Inst == null)
            {
                Inst = this;                   //実体を保存.
                DontDestroyOnLoad(gameObject); //Scene移動しても消さずに残す.

                RegistSound(); //サウンド登録.
            }
            else
            {
                Destroy(gameObject); //2つ目以降は消去.
            }
        }

        /// <summary>
        /// サウンド登録.
        /// </summary>
        private void RegistSound()
        {
            //BGM登録.
            foreach(var i in bgmData)
            {
                bgmClips.Add(i.Name, i.Audio);
            }
            //SE登録.
            foreach (var i in seData)
            {
                seClips.Add(i.Name, i.Audio);
            }
        }

        /// <summary>
        /// BGM再生.
        /// </summary>
        /// <param name="name">BGM登録名</param>
        public void PlayBGM(string name, bool isLoop)
        {
            //Dictionaryから値を取得.
            if (bgmClips.TryGetValue(name, out var bgm)) {
                audioSource.clip = bgm;    //取得したサウンドを入れる.
                audioSource.loop = isLoop; //ループ設定.
                audioSource.Play();        //再生.
            }
        }
        /// <summary>
        /// SE再生.
        /// </summary>
        /// <param name="name">SE登録名</param>
        public void PlaySE(string name)
        {
            //Dictionaryから値を取得.
            if (seClips.TryGetValue(name, out var se)) {
                audioSource.PlayOneShot(se); //取得したサウンドを再生.
            }
        }
        /// <summary>
        /// BGM停止.
        /// </summary>
        public void StopBGM()
        {
            audioSource.Stop(); //現在流してるBGMを停止.
        }
        /// <summary>
        /// 音量設定.
        /// </summary>
        public void SetVolume(float volume)
        {
            audioSource.volume = Mathf.Clamp(volume, 0f, 1f); //0.0～1.0の範囲で設定.
        }
    }
}