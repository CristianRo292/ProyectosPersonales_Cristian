using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UI : MonoBehaviour
{
    public AudioSource voz_historia;
    public GameObject btn_pley;
    public GameObject btn_pausar;

    // Start is called before the first frame update
    void Start()
    {
        voz_historia.Play();
        voz_historia.Pause();
        btn_pausar.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public void Reproducir_voz()
    {
        voz_historia.UnPause();
        btn_pausar.SetActive(true);
        btn_pley.SetActive(false);

    }
    public void Detener_voz()
    {
        voz_historia.Pause();
        btn_pausar.SetActive(false);
        btn_pley.SetActive(true);
    }
}
