using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class creadorEnemigos : MonoBehaviour
{
    public GameObject enemigos;
    public float timepo_aparicion = 1.0f;

    // Start is called before the first frame update
    void Start()
    {
        StartCoroutine(GenerarPoder());
    }

    // Update is called once per frame
    void Update()
    {

    }
    IEnumerator GenerarPoder()
    {
        while (true)
        {
            crear();
            yield return new WaitForSeconds(timepo_aparicion);
        }

    }
    private void crear()
    {
        if (enemigos == null)
        {
            return;
        }
        Camera cam = Camera.main;
        float alto = cam.orthographicSize;
        float ancho = alto * cam.aspect;
        float coordX = Random.Range(-ancho, ancho);
        float coordY = (Random.value > 0.5f) ? alto + 1 : -alto - 1;
        Vector3 posicion = new Vector3(coordX, 9.2f, -1);
        GameObject clon =  Instantiate(enemigos, posicion, Quaternion.identity);
        Destroy(clon, 10.0f);
    }
}
