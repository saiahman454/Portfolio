using UnityEngine;
using UnityEngine.SceneManagement;

public class LevelNav : MonoBehaviour
{
  public string scene1;
  public string scene2;
  // Update is called once per frame
  void Update()
  {
    if (Input.GetKeyUp(KeyCode.F1) && SceneManager.GetActiveScene().name != scene1)
    {
      SceneManager.LoadSceneAsync(scene1);
    }
    if (Input.GetKeyUp(KeyCode.F2) && SceneManager.GetActiveScene().name != scene2)
    {
      SceneManager.LoadSceneAsync(scene2);
    }

    //Reload the current Scene
    if (Input.GetKeyUp(KeyCode.R))
    {
      SceneManager.LoadScene(SceneManager.GetActiveScene().name);
    }
    //Quit the Game
    if (Input.GetKeyUp(KeyCode.Escape))
    {
      Application.Quit();
    }
  }
}