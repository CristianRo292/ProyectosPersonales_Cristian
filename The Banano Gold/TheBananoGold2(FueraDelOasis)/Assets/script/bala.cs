using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class bala : MonoBehaviour
{
    public float velocidad_Bala = 50f;
    public GameObject bala_ob;
    //public Transform armaIzq;
    //public Transform armaDer;
    public Transform[] naveNivel0;
    public Transform[] naveNivel1;
    public Transform[] naveNiveldanada;

    public AudioSource audioBala;

    public Sprite spriteNivel0;
    public Sprite spriteNivel1;
    public Sprite spriteNivelDanada;

    public SpriteRenderer sr;

    //private bool estModoNave = false;
    private int modoNave = 2;

    private float tiempoPoder = 5.0f;
    // Start is called before the first frame update
    void Start()
    {
        sr = GetComponent<SpriteRenderer>();
        ActivarArmaN0();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetButtonDown("Fire1"))
        {
            shoot();
            audioBala.Play();
        }

    }
    private void shoot()
    {
        //Transform[] actuales = !estModoNave ? naveNivel0 : naveNivel1;
        Transform[] actuales;
        if (modoNave == 1)
        {
            actuales = naveNiveldanada;
        }
        else if (modoNave == 3)
        {
            actuales = naveNivel1;
        }
        else
        {
            actuales = naveNivel0;
        }
        foreach (Transform t in actuales)
        {
            Disparar(t);
        }
    }
    private void Disparar(Transform arma)
    {

        GameObject clonBala = Instantiate(bala_ob, arma.position, arma.rotation);
        Rigidbody2D cuerpoBala = clonBala.GetComponent<Rigidbody2D>();
        cuerpoBala.velocity = arma.up * velocidad_Bala;
        Destroy(clonBala, 5f);
    }
    private void ActivarArmaN0()
    {
        //estModoNave = false;
        modoNave = 2;
        sr.sprite = spriteNivel0;
    }
    public void ActivarNaveDanada()
    {
        //estModoNave = false;
        modoNave = 1;
        sr.sprite = spriteNivelDanada;
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("poder"))
        {
            ActivarPoder(tiempoPoder);
        }
        else if (collision.CompareTag("enemigo") && modoNave != 3)
        {
            ActivarNaveDanada();
        }
            
        
    }
    private void ActivarPoder(float duracion)
    {
        //estModoNave = true;
        //sr.sprite = spriteNivel1;
        StopAllCoroutines();
        StartCoroutine(poderPersonaje(duracion));
    }
    IEnumerator poderPersonaje(float duracion)
    {
        //estModoNave = true;
        modoNave = 3;
        sr.sprite = spriteNivel1;
        yield return new WaitForSeconds(duracion);
        //estModoNave = false;
        modoNave = 2;
        sr.sprite = spriteNivel0;
    }
}
