using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;


public class CloudController : MonoBehaviour
{
  ParticleSystem system;
  public Color color;
  public Color lining;
  bool painted = false;
  public int numberOfParticles;
  public float minSpeed;
  public float maxSpeed;
  public float distance;
  Vector3 startPosition;
  float speed;


  // Start is called before the first frame update
  void Start()
  {
    system = GetComponent<ParticleSystem>();
    Spawn();
  }

  private void Spawn()
  {
    float xpos = Random.Range(-0.5f, 0.5f);
    float ypos = Random.Range(-0.5f, 0.5f);
    float zpos = Random.Range(-0.5f, 0.5f);
    transform.localPosition = new Vector3(xpos,ypos, zpos);
    speed = Random.Range(minSpeed, maxSpeed);
    startPosition = transform.position;
  }
  // Update is called once per frame
  void Update()
  {
    if(!painted)
    {
      Paint();
    }

    transform.Translate(0, 0, speed * UnityEngine.Time.deltaTime);
    if(Vector3.Distance(transform.position,startPosition) > distance)
    {
      Spawn();
    }
  }

  private void Paint()
  {
    ParticleSystem.Particle[] particles = new ParticleSystem.Particle[system.particleCount];
    system.GetParticles(particles);
    if(particles.Length > 0)
    {
      for(int i =0; i < particles.Length; i++)
      {
        particles[i].startColor = Color.Lerp(lining, color, particles[i].position.y / system.shape.scale.y);
      }
      painted = true;
      system.SetParticles(particles, particles.Length);
    }
  }
}
