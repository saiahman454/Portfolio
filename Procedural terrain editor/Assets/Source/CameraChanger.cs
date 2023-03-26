using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraChanger : MonoBehaviour
{
  GameObject player;
  GameObject Camera1;
  GameObject Camera2;
  GameObject Camera3;
  GameObject Camera4;
  private List<GameObject> cameras;
  private bool cinimaticMode = false;
  private int current_camera = 0;
  // Start is called before the first frame update
  void Start()
  {
    player = GameObject.Find("FirstPersonController");
    Camera1 = GameObject.Find("Camera1");
    Camera2 = GameObject.Find("Camera2");
    Camera3 = GameObject.Find("Camera3");
    Camera4 = GameObject.Find("Camera4");
    Camera1.SetActive(false);
    Camera2.SetActive(false);
    Camera3.SetActive(false);
    Camera4.SetActive(false);
    cameras = new List<GameObject>();
    cameras.Add(Camera1);
    cameras.Add(Camera2); 
    cameras.Add(Camera3);
    cameras.Add(Camera4);
  }

  // Update is called once per frame
  void Update()
  {
    if (Input.GetKeyDown(KeyCode.C))
    {
      if (cinimaticMode)
      {
        player.SetActive(true);
        cameras[current_camera].SetActive(false);
        cinimaticMode = false;
      }
      else
      {
        player.SetActive(false);
        cameras[current_camera].SetActive(true);
        cinimaticMode = true;
      }
    }
    if(Input.GetKeyDown(KeyCode.RightArrow))
    {
      if(cinimaticMode)
      {
        cameras[current_camera].SetActive(false);
        current_camera++;
        if(current_camera >= cameras.Count)
        {
          current_camera = 0;
        }
        cameras[current_camera].SetActive(true);
      }
    }
    if (Input.GetKeyDown(KeyCode.LeftArrow))
    {
      if (cinimaticMode)
      {
        cameras[current_camera].SetActive(false);
        current_camera--;
        if (current_camera < 0)
        {
          current_camera = cameras.Count-1;
        }
        cameras[current_camera].SetActive(true);
      }
    }
  }
}
