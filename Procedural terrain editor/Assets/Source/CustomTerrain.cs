using UnityEditor;
using UnityEngine;
using System.Collections.Generic;
using System.Linq;



[ExecuteInEditMode]

public class CustomTerrain : MonoBehaviour
{
  public Vector2 randomHeightRange = new Vector2(0, 0.1f);
  public Terrain terrain;

  //loading height map photos
  public TerrainData terrainData;
  public Texture2D heightMapImage;
  public Vector3 heightMapScale = new Vector3(1, 1, 1);

  public bool resetTerrain = true;

  //perline noise
  public float perlinFrequency = 0.01f;
  public float perlinAmplitude = 0.01f;
  public float perlinXOffset = 0.0f;
  public float perlinYOffset = 0.0f;
  public int perlinOctaves = 3;
  public float perlinPersistance = 8;
  public float perlinHeightScale = 0.09f;

  //Voronoi
  public float voronoiFallOff = 0.2f;
  public float voronoiDropOff = 0.6f;
  public float voronoiMinHeight = 0.1f;
  public float voronoiMaxHeight = 0.5f;
  public int voronoiPeaks = 5;
  public enum VoronoiType { Linear = 0, Power, Combined, SinPow };
  public VoronoiType voronoiType = VoronoiType.Combined;

  //Mid Point Displacment
  public float MPDheightMin = -2f;
  public float MPDheightMax = 2f;
  public float MPDheightDampenerPower = 2.0f;
  public float MPDroughness = 2.0f;
  public int SmoothRepeat = 5;

  //multiple perlin noise
  [System.Serializable]
  public class PerlineParams
  {
    public float XScale = 0.01f;
    public float YScale = 0.01f;
    public float XOffset = 0.0f;
    public float YOffset = 0.0f;
    public int Octaves = 3;
    public float Persistance = 8;
    public float HeightScale = 0.09f;
    public bool remove = false;
  }

  public List<PerlineParams> perlineParams = new List<PerlineParams>()
  {
    new PerlineParams()
  };

  //Terrain Layers
  public float terrianOffset = 0.1f;
  public float terrainNoiseX = 0.01f;
  public float terrainNoiseY = 0.01f;
  public float terrainNoiseScaler = 0.1f;


  [System.Serializable]
  public class TerrainHeights
  {
    public Texture2D texture = null;
    public float minHeight = 0.1f;
    public float maxHeight = 0.2f;
    public float minSlope = 0;
    public float maxSlope = 1.5f;
    public Vector2 tileOffset = new Vector2(0, 0);
    public Vector2 tileSize = new Vector2(50, 50);
    public bool remove = false;
  }

  public List<TerrainHeights> terrainHeights = new List<TerrainHeights>()
  {
    new TerrainHeights()
  };


  //Vegetation
  [System.Serializable]
  public class Vegetation
  {
    public GameObject mesh;
    public float minHeight = 0.1f;
    public float maxHeight = 0.2f;
    public float minSlope = 0;
    public float maxSlope = 90;
    public float minScale = 0.5f;
    public float maxScale = 1.0f;
    public Color colour1 = Color.white;
    public Color colour2 = Color.white;
    public Color lightColour = Color.white;
    public float minRotation = 0;
    public float maxRotation = 360;
    public float density = 0.5f;
    public bool remove = false;
  }

  public List<Vegetation> vegetation = new List<Vegetation>()
    {
        new Vegetation()
    };

  public int maxTrees = 5000;
  public int treeSpacing = 5;

  //Props
  [System.Serializable]
  public class Detail
  {
    public GameObject prototype = null;
    public Texture2D prototypeTexture = null;
    public float minHeight = 0.1f;
    public float maxHeight = 0.2f;
    public float minSlope = 0;
    public float maxSlope = 1;
    public Color dryColour = Color.white;
    public Color healthyColour = Color.white;
    public Vector2 heightRange = new Vector2(1, 1);
    public Vector2 widthRange = new Vector2(1, 1);
    public float noiseSpread = 0.5f;
    public float overlap = 0.01f;
    public float feather = 0.05f;
    public float density = 0.5f;
    public bool remove = false;
  }

  public List<Detail> details = new List<Detail>() {
        new Detail()
    };

  public int maxDetails = 5000;
  public int detailSpacing = 5;


  //Water
  public float waterHeight = 0.5f;
  public GameObject waterGO;
  public Material shoreLineMaterial;

  //Erosion
  public enum ErosionType { Rain, Thermal, Tidal, River, Wind, Canyon };

  public ErosionType erosionType = ErosionType.Rain;
  public float erosionStrength = 0.1f;
  public int riversPerDroplet = 5;
  public float solubility = 0.01f;
  public int droplets = 10;
  public int erosionSmoothAmount = 5;
  public float thermalStrength = 0.01f;
  public float windDirection = 0;

  //Clouds
  public int numClouds = 1;
  public int particlesPerCloud = 50;
  public Material cloudMaterial;
  public Material cloudShadowMaterial;
  public float cloudStartSize = 5;
  public Color cloudColor = Color.white;
  public Color cloudLining = Color.grey;
  public float cloudMinSpeed = 0.2f;
  public float cloudMaxSpeed = 0.5f;
  public float cloudRange = 500.0f;
  public Vector3 cloudScaleMin = new Vector3(1, 1, 1);
  public Vector3 cloudScaleMax = new Vector3(1, 1, 1);

  public void GenerateClouds()
  {
    GameObject cloudManager = GameObject.Find("CloudManager");
    if (!cloudManager)
    {
      cloudManager = new GameObject();
      cloudManager.name = "CloudManager";
      cloudManager.AddComponent<CloudManager>();
      cloudManager.transform.position = transform.position;
    }
    GameObject[] allClouds = GameObject.FindGameObjectsWithTag("Cloud");
    for (int i = 0; i < allClouds.Length; i++)
    {
      DestroyImmediate(allClouds[i]);
    }
    for (int c = 0; c < numClouds; ++c)
    {
      GameObject cloudGO = new GameObject();
      cloudGO.name = "Cloud" + c;
      cloudGO.tag = "Cloud";
      cloudGO.transform.position = cloudManager.transform.position;
      cloudGO.transform.rotation = cloudManager.transform.rotation;
      CloudController cc = cloudGO.AddComponent<CloudController>();

      cc.lining = cloudLining;
      cc.color = cloudColor;
      cc.numberOfParticles = particlesPerCloud;
      cc.minSpeed = cloudMinSpeed;
      cc.maxSpeed = cloudMaxSpeed;
      cc.distance = cloudRange;

      cloudGO.layer = LayerMask.NameToLayer("Sky");
      if (UnityEngine.Random.Range(0, 11) > 5)
      {
        GameObject cloudProjector = new GameObject();
        cloudProjector.name = "Shadow";
        cloudProjector.transform.position = cloudGO.transform.position;
        cloudProjector.transform.forward = Vector3.down;
        cloudProjector.transform.parent = cloudGO.transform;
        Projector cp = cloudProjector.AddComponent<Projector>();
        cp.material = cloudShadowMaterial;
        cp.farClipPlane = terrainData.size.y;
        int skyLayerMask = 1 << LayerMask.NameToLayer("Sky");
        int waterLayerMask = 1 << LayerMask.NameToLayer("Water");
        cp.ignoreLayers = skyLayerMask | waterLayerMask;
        cp.fieldOfView = 20.0f;
      }

      ParticleSystem cloudsystem = cloudGO.AddComponent<ParticleSystem>();
      Renderer cloudRend = cloudGO.GetComponent<Renderer>();
      cloudRend.material = cloudMaterial;
      cloudRend.shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.Off;
      cloudRend.receiveShadows = false;
      ParticleSystem.MainModule main = cloudsystem.main;
      main.loop = false;
      main.startLifetime = 10000000;
      main.startSpeed = 0;
      main.startSize = cloudStartSize;
      main.startColor = Color.white;

      var emission = cloudsystem.emission;
      emission.rateOverTime = 0;
      emission.SetBursts(new ParticleSystem.Burst[] { new ParticleSystem.Burst(0.0f, (short)particlesPerCloud) });
      var shape = cloudsystem.shape;
      shape.shapeType = ParticleSystemShapeType.Sphere;
      float rand_x = UnityEngine.Random.Range(cloudScaleMin.x, cloudScaleMax.x);
      float rand_y = UnityEngine.Random.Range(cloudScaleMin.y, cloudScaleMax.y);
      float rand_z = UnityEngine.Random.Range(cloudScaleMin.z, cloudScaleMax.z);
      shape.scale = new Vector3(rand_x, rand_y, rand_z);

      cloudGO.transform.parent = cloudManager.transform;
      cloudGO.transform.localScale = new Vector3(1, 1, 1);
    }

  }

  public void Erode()
  {
    switch (erosionType)
    {
      case ErosionType.Rain:
        {
          Rain();
          break;
        }
      case ErosionType.Thermal:
        {
          Thermal();
          break;
        }
      case ErosionType.Tidal:
        {
          Tidal();
          break;
        }
      case ErosionType.River:
        {
          River();
          break;
        }
      case ErosionType.Wind:
        {
          Wind();
          break;
        }
      case ErosionType.Canyon:
        {
          DigCanyon();
          break;
        }
    }

    SmoothRepeat = erosionSmoothAmount;
    Smooth();
  }

  float[,] tempHeightMap;
  public void DigCanyon()
  {
    int res = terrainData.heightmapResolution;
    float digDepth = erosionStrength;
    float bankSlope = thermalStrength;
    float maxDepth = 0;
    tempHeightMap = terrainData.GetHeights(0, 0, res, res);
    int cx = 1;
    int cy = UnityEngine.Random.Range(10, res - 10);
    while (cy >= 0 && cy < res && cx > 0 && cx < res)
    {
      CanyonCrawler(cx, cy, tempHeightMap[cx, cy] - digDepth, bankSlope, maxDepth);
      cx = cx + UnityEngine.Random.Range(-1, 4);
      cy = cy + UnityEngine.Random.Range(-2, 3);
    }
    terrainData.SetHeights(0, 0, tempHeightMap);
  }

  void CanyonCrawler(int x, int y, float height, float slope, float maxDepth)
  {
    int res = terrainData.heightmapResolution;
    if (x < 0 || x >= res) return; //off x range of map
    if (y < 0 || y >= res) return; //off y range of map
    if (height <= maxDepth) return; //if hit lowest level
    if (tempHeightMap[x, y] <= height) return; //if run into lower elevation

    tempHeightMap[x, y] = height;

    CanyonCrawler(x + 1, y, height + UnityEngine.Random.Range(slope, slope + 0.01f), slope, maxDepth);
    CanyonCrawler(x - 1, y, height + UnityEngine.Random.Range(slope, slope + 0.01f), slope, maxDepth);
    CanyonCrawler(x + 1, y + 1, height + UnityEngine.Random.Range(slope, slope + 0.01f), slope, maxDepth);
    CanyonCrawler(x - 1, y + 1, height + UnityEngine.Random.Range(slope, slope + 0.01f), slope, maxDepth);
    CanyonCrawler(x, y - 1, height + UnityEngine.Random.Range(slope, slope + 0.01f), slope, maxDepth);
    CanyonCrawler(x, y + 1, height + UnityEngine.Random.Range(slope, slope + 0.01f), slope, maxDepth);
  }



  private void Wind()
  {
    int res = terrainData.heightmapResolution;
    float[,] heightMap = terrainData.GetHeights(0, 0, res, res);

    float sinAngle = -Mathf.Sin(Mathf.Deg2Rad * windDirection);
    float cosAngle = Mathf.Cos(Mathf.Deg2Rad * windDirection);

    for (int y = -(res - 1) * 2; y <= res * 2; y += 10)
    {
      for (int x = -(res - 1) * 2; x <= res * 2; ++x)
      {
        int Noise = (int)(Mathf.PerlinNoise(x * 0.06f, y * 0.06f) * 20 * erosionStrength);
        int nx = x;
        int digy = y + Noise;
        int ny = y + 5 + Noise;

        Vector2 digPos = new Vector2(x * cosAngle - digy * sinAngle, digy * cosAngle + x * sinAngle);
        Vector2 pilePos = new Vector2(nx * cosAngle - ny * sinAngle, ny * cosAngle + nx * sinAngle);
        if (!((int)digPos.x < 0 || (int)digPos.x > (res - 1) || (int)digPos.y < 0 || (int)digPos.y > (res - 1) ||
             (int)pilePos.x < 0 || (int)pilePos.x > (res - 1) || (int)pilePos.y < 0 || (int)pilePos.y > (res - 1)))
        {
          heightMap[(int)digPos.x, (int)digPos.y] -= 0.001f;
          heightMap[(int)pilePos.x, (int)pilePos.y] += 0.001f;
        }
      }
    }
    terrainData.SetHeights(0, 0, heightMap);
  }


  private void Rain()
  {
    int res = terrainData.heightmapResolution;
    float[,] heightMap = terrainData.GetHeights(0, 0, res, res);

    for (int i = 0; i < droplets; ++i)
    {
      int rand_x = UnityEngine.Random.Range(0, res);
      int rand_y = UnityEngine.Random.Range(0, res);
      heightMap[rand_x, rand_y] -= erosionStrength;
    }

    terrainData.SetHeights(0, 0, heightMap);
  }

  private void Thermal()
  {
    int res = terrainData.heightmapResolution;
    float[,] heightMap = terrainData.GetHeights(0, 0, res, res);

    for (int y = 0; y < res; ++y)
    {
      for (int x = 0; x < res; ++x)
      {
        List<Vector2> neighbours = GetNeighbours(x, y);
        foreach (Vector2 n in neighbours)
        {
          if (heightMap[x, y] > heightMap[(int)n.x, (int)n.y] + erosionStrength)
          {
            float currentHeight = heightMap[x, y];
            heightMap[x, y] -= currentHeight * thermalStrength;
            heightMap[(int)n.x, (int)n.y] += currentHeight * thermalStrength;
          }
        }
      }
    }
    terrainData.SetHeights(0, 0, heightMap);
  }

  private void River()
  {
    int res = terrainData.heightmapResolution;
    float[,] heightMap = terrainData.GetHeights(0, 0, res, res);
    float[,] erosionMap = new float[res, res];
    for (int i = 0; i < droplets; ++i)
    {
      int rand_x = UnityEngine.Random.Range(0, res);
      int rand_y = UnityEngine.Random.Range(0, res);
      Vector2 dropletPos = new Vector2(rand_x, rand_y);
      erosionMap[(int)dropletPos.x, (int)dropletPos.y] = erosionStrength;
      for (int j = 0; j < riversPerDroplet; ++j)
      {
        erosionMap = RunRiver(dropletPos, heightMap, erosionMap, res);
      }
    }


    for (int y = 0; y < res; ++y)
    {
      for (int x = 0; x < res; ++x)
      {
        if (erosionMap[x, y] > 0)
        {
          heightMap[x, y] -= erosionMap[x, y];
        }
      }
    }
    terrainData.SetHeights(0, 0, heightMap);
  }
  private float[,] RunRiver(Vector3 dropletPos, float[,] heightMap, float[,] erosionMap, int res)
  {
    while (erosionMap[(int)dropletPos.x, (int)dropletPos.y] > 0)
    {
      List<Vector2> neighbours = GetNeighbours((int)dropletPos.x, (int)dropletPos.y);
      neighbours.Shuffle();
      bool foundLower = false;
      foreach (Vector2 n in neighbours)
      {
        if (heightMap[(int)n.x, (int)n.y] < heightMap[(int)dropletPos.x, (int)dropletPos.y])
        {
          erosionMap[(int)n.x, (int)n.y] = erosionMap[(int)dropletPos.x, (int)dropletPos.y] - solubility;
          dropletPos = n;
          foundLower = true;
          break;
        }
      }
      if (!foundLower)
      {
        erosionMap[(int)dropletPos.x, (int)dropletPos.y] -= solubility;
      }
    }
    return erosionMap;
  }
  private void Tidal()
  {
    int res = terrainData.heightmapResolution;
    float[,] heightMap = terrainData.GetHeights(0, 0, res, res);
    for (int y = 0; y < res; ++y)
    {
      for (int x = 0; x < res; ++x)
      {
        List<Vector2> neighbours = GetNeighbours(x, y);
        foreach (Vector2 n in neighbours)
        {
          if ((heightMap[x, y] < waterHeight) && (heightMap[(int)n.x, (int)n.y] > waterHeight))
          {
            heightMap[x, y] = waterHeight;
            heightMap[(int)n.x, (int)n.y] = waterHeight;
          }
        }
      }
    }
    terrainData.SetHeights(0, 0, heightMap);
  }





  public void DrawShoreLine()
  {
    float[,] heightMap = terrainData.GetHeights(0, 0, terrainData.heightmapResolution, terrainData.heightmapResolution);
    //int quadcount = 0;
    //GameObject quads = new GameObject("QUADS");
    for (int y = 0; y < terrainData.heightmapResolution; ++y)
    {
      for (int x = 0; x < terrainData.heightmapResolution; ++x)
      {
        List<Vector2> neighbours = GetNeighbours(x, y);
        foreach (Vector2 n in neighbours)
        {
          //if im below water line and my neighbor is above then im on the shore
          if (heightMap[x, y] < waterHeight && heightMap[(int)n.x, (int)n.y] > waterHeight)
          {
            //if (quadcount < 1000)
            //{
            //quadcount++;
            GameObject go = GameObject.CreatePrimitive(PrimitiveType.Quad);
            go.transform.localScale *= 10.0f;
            go.transform.position = transform.position +
              new Vector3(y / (float)terrainData.heightmapResolution * terrainData.size.z,
              waterHeight * terrainData.size.y,
              x / (float)terrainData.heightmapResolution * terrainData.size.x);
            go.transform.LookAt(new Vector3(n.y / (float)terrainData.heightmapResolution * terrainData.size.z,
              waterHeight * terrainData.size.y,
              n.x / (float)terrainData.heightmapResolution * terrainData.size.x));
            go.transform.Rotate(90, 0, 0);
            go.tag = "Shore";
            //go.transform.parent = quads.transform;
            //}
          }
        }
      }
    }
    GameObject[] shoreQuads = GameObject.FindGameObjectsWithTag("Shore");
    MeshFilter[] meshFilters = new MeshFilter[shoreQuads.Length];
    for (int m = 0; m < shoreQuads.Length; ++m)
    {
      meshFilters[m] = shoreQuads[m].GetComponent<MeshFilter>();
    }
    CombineInstance[] combine = new CombineInstance[meshFilters.Length];
    int length = meshFilters.Length;
    for (int i = 0; i < length; ++i)
    {
      combine[i].mesh = meshFilters[i].sharedMesh;
      combine[i].transform = meshFilters[i].transform.localToWorldMatrix;
      meshFilters[i].gameObject.SetActive(false);
    }
    GameObject currentShoreLine = GameObject.Find("ShoreLine");
    if (currentShoreLine)
    {
      DestroyImmediate(currentShoreLine);
    }
    GameObject shoreLine = new GameObject();
    shoreLine.name = "ShoreLine";
    shoreLine.AddComponent<WaveAnimation>();
    shoreLine.transform.position = transform.position;
    shoreLine.transform.rotation = transform.rotation;
    MeshFilter thisMF = shoreLine.AddComponent<MeshFilter>();
    thisMF.mesh = new Mesh();
    shoreLine.GetComponent<MeshFilter>().sharedMesh.CombineMeshes(combine);
    //stop cloud shadows
    shoreLine.layer = LayerMask.NameToLayer("Water");
    MeshRenderer r = shoreLine.AddComponent<MeshRenderer>();
    r.sharedMaterial = shoreLineMaterial;

    for (int i = 0; i < shoreQuads.Length; ++i)
    {
      DestroyImmediate(shoreQuads[i]);
    }


  }

  public void AddWater()
  {
    GameObject water = GameObject.Find("water");
    if (!water)
    {
      water = Instantiate(waterGO, transform.position, transform.rotation);
      water.name = "water";
    }
    water.transform.position = transform.position + new Vector3(terrainData.size.x / 2, terrainData.size.y * waterHeight, terrainData.size.z / 2);
    water.transform.localScale = new Vector3(terrainData.size.x, 1, terrainData.size.z);

  }

  public void AddDetails()
  {
    DetailPrototype[] newDetailPrototypes;
    newDetailPrototypes = new DetailPrototype[details.Count];
    int dIndex = 0;
    float[,] heightMap = terrainData.GetHeights(0, 0, terrainData.heightmapResolution,
                                        terrainData.heightmapResolution);

    foreach (Detail d in details)
    {
      newDetailPrototypes[dIndex] = new DetailPrototype();
      newDetailPrototypes[dIndex].prototype = d.prototype;
      newDetailPrototypes[dIndex].prototypeTexture = d.prototypeTexture;
      newDetailPrototypes[dIndex].healthyColor = d.healthyColour;
      newDetailPrototypes[dIndex].dryColor = d.dryColour;
      newDetailPrototypes[dIndex].minHeight = d.heightRange.x;
      newDetailPrototypes[dIndex].maxHeight = d.heightRange.y;
      newDetailPrototypes[dIndex].minWidth = d.widthRange.x;
      newDetailPrototypes[dIndex].maxWidth = d.widthRange.y;
      newDetailPrototypes[dIndex].noiseSpread = d.noiseSpread;

      if (newDetailPrototypes[dIndex].prototype)
      {
        newDetailPrototypes[dIndex].usePrototypeMesh = true;
        newDetailPrototypes[dIndex].renderMode = DetailRenderMode.VertexLit;
      }
      else
      {
        newDetailPrototypes[dIndex].usePrototypeMesh = false;
        newDetailPrototypes[dIndex].renderMode = DetailRenderMode.GrassBillboard;
      }
      dIndex++;
    }
    terrainData.detailPrototypes = newDetailPrototypes;

    for (int i = 0; i < terrainData.detailPrototypes.Length; ++i)
    {
      int[,] detailMap = new int[terrainData.detailWidth, terrainData.detailHeight];
      for (int y = 0; y < terrainData.detailHeight; y += detailSpacing)
      {
        for (int x = 0; x < terrainData.detailWidth; x += detailSpacing)
        {
          if (UnityEngine.Random.Range(0.0f, 1.0f) > details[i].density) continue;

          int xHM = (int)(x / (float)terrainData.detailWidth * terrainData.heightmapResolution);
          int yHM = (int)(y / (float)terrainData.detailHeight * terrainData.heightmapResolution);

          float thisNoise = Utils.Map(Mathf.PerlinNoise(x * details[i].feather,
                                      y * details[i].feather), 0, 1, 0.5f, 1);
          float thisHeightStart = details[i].minHeight * thisNoise -
                                  details[i].overlap * thisNoise;
          float nextHeightStart = details[i].maxHeight * thisNoise +
                                  details[i].overlap * thisNoise;

          float thisHeight = heightMap[yHM, xHM];
          float steepness = terrainData.GetSteepness(xHM / (float)terrainData.size.x,
                                                      yHM / (float)terrainData.size.z);
          if ((thisHeight >= thisHeightStart && thisHeight <= nextHeightStart) &&
              (steepness >= details[i].minSlope && steepness <= details[i].maxSlope))
          {
            detailMap[y, x] = 1;
          }
        }
      }
      terrainData.SetDetailLayer(0, 0, i, detailMap);
    }
  }

  public void AddNewDetails()
  {
    details.Add(new Detail());
  }

  public void RemoveDetails()
  {
    List<Detail> keptDetails = new List<Detail>();
    for (int i = 0; i < details.Count; ++i)
    {
      if (!details[i].remove)
      {
        keptDetails.Add(details[i]);
      }
    }
    if (keptDetails.Count == 0)
    {    // Don't want to keep any
      keptDetails.Add(details[0]);  // Add at least one;
    }
    details = keptDetails;
  }

  public void PlantVegetation()
  {
    TreePrototype[] newTreePrototypes;
    newTreePrototypes = new TreePrototype[vegetation.Count];
    int tindex = 0;
    foreach (Vegetation t in vegetation)
    {
      newTreePrototypes[tindex] = new TreePrototype();
      newTreePrototypes[tindex].prefab = t.mesh;
      tindex++;
    }
    terrainData.treePrototypes = newTreePrototypes;

    List<TreeInstance> allVegetation = new List<TreeInstance>();
    for (int z = 0; z < terrainData.size.z; z += treeSpacing)
    {
      for (int x = 0; x < terrainData.size.x; x += treeSpacing)
      {
        for (int tp = 0; tp < terrainData.treePrototypes.Length; tp++)
        {
          if (UnityEngine.Random.Range(0.0f, 1.0f) > vegetation[tp].density)
          {
            continue;
          }

          float thisHeight = terrainData.GetHeight(x, z) / terrainData.size.y;
          float thisHeightStart = vegetation[tp].minHeight;
          float thisHeightEnd = vegetation[tp].maxHeight;

          float steepness = terrainData.GetSteepness(x / (float)terrainData.size.x, z / (float)terrainData.size.z);

          if ((thisHeight >= thisHeightStart && thisHeight <= thisHeightEnd) &&
              (steepness >= vegetation[tp].minSlope && steepness <= vegetation[tp].maxSlope))
          {
            TreeInstance instance = new TreeInstance();
            float x_rand = UnityEngine.Random.Range(-5.0f, 5.0f);
            float z_rand = UnityEngine.Random.Range(-5.0f, 5.0f);
            instance.position = new Vector3((x + x_rand) / terrainData.size.x,
                                            terrainData.GetHeight(x, z) / terrainData.size.y,
                                            (z + z_rand) / terrainData.size.z);

            Vector3 treeWorldPos = new Vector3(instance.position.x * terrainData.size.x,
                instance.position.y * terrainData.size.y,
                instance.position.z * terrainData.size.z)
                                             + this.transform.position;

            RaycastHit hit;
            int layerMask = 1 << terrainLayer;

            if (Physics.Raycast(treeWorldPos + new Vector3(0, 10, 0), -Vector3.up, out hit, 100, layerMask) ||
                Physics.Raycast(treeWorldPos - new Vector3(0, 10, 0), Vector3.up, out hit, 100, layerMask))
            {
              float treeHeight = (hit.point.y - this.transform.position.y) / terrainData.size.y;
              instance.position = new Vector3(instance.position.x, treeHeight, instance.position.z);
              instance.rotation = UnityEngine.Random.Range(vegetation[tp].minRotation, vegetation[tp].maxRotation);
              instance.prototypeIndex = tp;
              instance.color = Color.Lerp(vegetation[tp].colour1, vegetation[tp].colour2, UnityEngine.Random.Range(0.0f, 1.0f));
              instance.lightmapColor = vegetation[tp].lightColour;
              float scale = UnityEngine.Random.Range(vegetation[tp].minScale, vegetation[tp].maxScale);
              instance.heightScale = scale;
              instance.widthScale = scale;

              allVegetation.Add(instance);
              if (allVegetation.Count >= maxTrees)
              {
                goto TREESDONE;
              }
            }

          }
        }
      }
    }
  TREESDONE:
    terrainData.treeInstances = allVegetation.ToArray();

  }
  public void AddNewVegetation()
  {
    vegetation.Add(new Vegetation());
  }

  public void RemoveVegetation()
  {
    List<Vegetation> keptVegetation = new List<Vegetation>();
    for (int i = 0; i < vegetation.Count; i++)
    {
      if (!vegetation[i].remove)
      {
        keptVegetation.Add(vegetation[i]);
      }
    }
    if (keptVegetation.Count == 0) //don't want to keep any
    {
      keptVegetation.Add(vegetation[0]); //add at least 1
    }
    vegetation = keptVegetation;
  }


  public void AddTerrainLayer()
  {
    terrainHeights.Add(new TerrainHeights());
  }

  public void RemoveTerrainLayer()
  {
    List<TerrainHeights> kept = new List<TerrainHeights>();
    for (int i = 0; i < terrainHeights.Count; ++i)
    {
      if (!terrainHeights[i].remove)
      {
        kept.Add(terrainHeights[i]);
      }
    }
    if (kept.Count == 0)
    {
      kept.Add(terrainHeights[0]);
    }
    terrainHeights = kept;
  }
  //normalize variable number vetor
  private void NormalizeVector(float[] vec)
  {
    float total = 0;
    for (int i = 0; i < vec.Length; ++i)
    {
      total += vec[i];
    }
    for (int i = 0; i < vec.Length; ++i)
    {
      vec[i] /= total;
    }
  }

  private float GetSteepness(float[,] heightmap, int x, int y, int width, int height)
  {
    float h = heightmap[x, y];
    int nx = x + 1;
    int ny = y + 1;

    if (nx > width - 1)
    {
      nx = x - 1;
    }
    if (ny > height - 1)
    {
      ny = y - 1;
    }
    float dx = heightmap[nx, y] - h;
    float dy = heightmap[x, ny] - h;
    Vector2 gradient = new Vector2(dx, dy);

    float steep = gradient.magnitude;
    return steep;
  }
  public void TerrainMaps()
  {
    TerrainLayer[] terrainLayers;
    terrainLayers = new TerrainLayer[terrainHeights.Count()];
    int tlindex = 0;
    foreach (TerrainHeights layer in terrainHeights)
    {
      terrainLayers[tlindex] = new TerrainLayer();
      terrainLayers[tlindex].diffuseTexture = layer.texture;
      terrainLayers[tlindex].tileOffset = layer.tileOffset;
      terrainLayers[tlindex].tileSize = layer.tileSize;
      terrainLayers[tlindex].diffuseTexture.Apply(true);
      string name = new string(terrainLayers[tlindex].diffuseTexture.name);
      AssetDatabase.CreateAsset(terrainLayers[tlindex], "Assets/TerrainLayers/" + name + ".terrainlayer");
      tlindex++;
    }
    terrainData.terrainLayers = terrainLayers;

    float[,] heightMap = terrainData.GetHeights(0, 0, terrainData.heightmapResolution, terrainData.heightmapResolution);

    float[,,] terrainLayerData = new float[terrainData.alphamapWidth,
      terrainData.alphamapHeight, terrainData.alphamapLayers];

    for (int y = 0; y < terrainData.alphamapHeight; ++y)
    {
      for (int x = 0; x < terrainData.alphamapWidth; ++x)
      {
        float[] splat = new float[terrainData.alphamapLayers];
        for (int i = 0; i < terrainHeights.Count(); ++i)
        {
          float noise = Mathf.PerlinNoise(x * terrainNoiseX, y * terrainNoiseY) * terrainNoiseScaler;
          float offset = terrianOffset + noise;
          float thisHeightStart = terrainHeights[i].minHeight - offset;
          float thisHeightEnd = terrainHeights[i].maxHeight + offset;
          //float steepness = GetSteepness(heightMap, x, y, terrainData.heightmapResolution, terrainData.heightmapResolution);
          float steepness = terrainData.GetSteepness(y / (float)terrainData.alphamapHeight, x / (float)terrainData.alphamapWidth);
          if (heightMap[x, y] >= thisHeightStart && heightMap[x, y] <= thisHeightEnd &&
            steepness >= terrainHeights[i].minSlope && steepness <= terrainHeights[i].maxSlope)
          {
            splat[i] = 1;
          }
        }
        NormalizeVector(splat);
        for (int j = 0; j < terrainHeights.Count(); ++j)
        {
          terrainLayerData[x, y, j] = splat[j];
        }
      }
    }
    terrainData.SetAlphamaps(0, 0, terrainLayerData);
  }

  float[,] GetHeightMap()
  {
    int height = terrainData.heightmapResolution;
    if (!resetTerrain)
    {
      return terrainData.GetHeights(0, 0, height, height);
    }
    else
      return new float[height, height];
  }

  List<Vector2> GetNeighbours(int x, int y)
  {
    List<Vector2> toReturn = new List<Vector2>();
    for (int i = x - 1; i < x + 2; ++i)
    {
      for (int j = y - 1; j < y + 2; ++j)
      {
        if (!(x == i && y == j))
        {
          if (IsValidGridPosition(i, j))
          {
            toReturn.Add(new Vector2(i, j));
          }
        }
      }
    }
    return toReturn;
  }
  private bool IsValidGridPosition(int x, int y)
  {
    if (x < 0 || y < 0 ||
      x > terrainData.heightmapResolution - 1 ||
      y > terrainData.heightmapResolution - 1)
    {
      return false;
    }
    return true;
  }
  public void Smooth()
  {
    float[,] heightmap = GetHeightMap();
    float smoothprog = 0;
    EditorUtility.DisplayProgressBar("Smoothing Terrain", "Progress", smoothprog);
    for (int r = 0; r < SmoothRepeat; ++r)
    {
      for (int y = 0; y < terrainData.heightmapResolution; ++y)
      {
        for (int x = 0; x < terrainData.heightmapResolution; ++x)
        {
          float avgHeight = 0;
          float count = 0;
          //go through all nieghbors not including self
          for (int i = x - 1; i < x + 2; ++i)
          {
            for (int j = y - 1; j < y + 2; ++j)
            {
              if (!(x == i && y == j))
              {
                if (IsValidGridPosition(i, j))
                {
                  avgHeight += heightmap[i, j];
                  count++;
                }
              }
            }
          }
          avgHeight /= count;
          heightmap[x, y] = avgHeight;
        }
      }
      smoothprog++;
      EditorUtility.DisplayProgressBar("Smoothing Terrain", "Progress", smoothprog / SmoothRepeat);
    }
    terrainData.SetHeights(0, 0, heightmap);
    EditorUtility.ClearProgressBar();
  }


  public void MPD()
  {
    float[,] heightMap = GetHeightMap();
    int width = terrainData.heightmapResolution - 1;
    int squareSize = width;
    float heightMin = MPDheightMin;
    float heightMax = MPDheightMax;
    float heightDampener = (float)Mathf.Pow(MPDheightDampenerPower, -1 * MPDroughness);


    int cornerX, cornerY;
    int midX, midY;
    int pmidXL, pmidXR, pmidYU, pmidYD;

    /* heightMap[0, 0] = UnityEngine.Random.Range(0f, 0.2f);
     heightMap[0, terrainData.heightmapHeight - 2] = UnityEngine.Random.Range(0f, 0.2f);
     heightMap[terrainData.heightmapWidth - 2, 0] = UnityEngine.Random.Range(0f, 0.2f);
     heightMap[terrainData.heightmapWidth - 2, terrainData.heightmapHeight - 2] = 
                                                 UnityEngine.Random.Range(0f, 0.2f);*/
    while (squareSize > 0)
    {
      for (int x = 0; x < width; x += squareSize)
      {
        for (int y = 0; y < width; y += squareSize)
        {
          cornerX = (x + squareSize);
          cornerY = (y + squareSize);

          midX = (int)(x + squareSize / 2.0f);
          midY = (int)(y + squareSize / 2.0f);

          heightMap[midX, midY] = (float)((heightMap[x, y] +
                                           heightMap[cornerX, y] +
                                           heightMap[x, cornerY] +
                                           heightMap[cornerX, cornerY]) / 4.0f +
                                          UnityEngine.Random.Range(heightMin, heightMax));
        }
      }

      for (int x = 0; x < width; x += squareSize)
      {
        for (int y = 0; y < width; y += squareSize)
        {

          cornerX = (x + squareSize);
          cornerY = (y + squareSize);

          midX = (int)(x + squareSize / 2.0f);
          midY = (int)(y + squareSize / 2.0f);

          pmidXR = (int)(midX + squareSize);
          pmidYU = (int)(midY + squareSize);
          pmidXL = (int)(midX - squareSize);
          pmidYD = (int)(midY - squareSize);

          if (pmidXL <= 0 || pmidYD <= 0
              || pmidXR >= width - 1 || pmidYU >= width - 1) continue;

          //Calculate the square value for the bottom side  
          heightMap[midX, y] = (float)((heightMap[midX, midY] +
                                        heightMap[x, y] +
                                        heightMap[midX, pmidYD] +
                                        heightMap[cornerX, y]) / 4.0f +
                                       UnityEngine.Random.Range(heightMin, heightMax));
          //Calculate the square value for the top side   
          heightMap[midX, cornerY] = (float)((heightMap[x, cornerY] +
                                                  heightMap[midX, midY] +
                                                  heightMap[cornerX, cornerY] +
                                              heightMap[midX, pmidYU]) / 4.0f +
                                             UnityEngine.Random.Range(heightMin, heightMax));

          //Calculate the square value for the left side   
          heightMap[x, midY] = (float)((heightMap[x, y] +
                                                  heightMap[pmidXL, midY] +
                                                  heightMap[x, cornerY] +
                                        heightMap[midX, midY]) / 4.0f +
                                       UnityEngine.Random.Range(heightMin, heightMax));
          //Calculate the square value for the right side   
          heightMap[cornerX, midY] = (float)((heightMap[cornerX, y] +
                                                  heightMap[midX, midY] +
                                                  heightMap[cornerX, cornerY] +
                                                  heightMap[pmidXR, midY]) / 4.0f +
                                             UnityEngine.Random.Range(heightMin, heightMax));

        }
      }

      squareSize = (int)(squareSize / 2.0f);
      heightMin *= heightDampener;
      heightMax *= heightDampener;
    }

    terrainData.SetHeights(0, 0, heightMap);
  }
  public void Voronoi()
  {
    float[,] heightMap = GetHeightMap();
    for (int p = 0; p < voronoiPeaks; ++p)
    {

      Vector3 peak = new Vector3(UnityEngine.Random.Range(0, terrainData.heightmapResolution),
        UnityEngine.Random.Range(voronoiMinHeight, voronoiMaxHeight),
        UnityEngine.Random.Range(0, terrainData.heightmapResolution));
      //prevents putting low peaking in a mountain
      if (heightMap[(int)peak.x, (int)peak.z] < peak.y)
      {
        heightMap[(int)peak.x, (int)peak.z] = peak.y;
      }
      else
      {
        continue;
      }
      Vector2 peakLocation = new Vector2((int)peak.x, (int)peak.z);
      float maxDistance = Vector2.Distance(new Vector2(0, 0),
        new Vector2(terrainData.heightmapResolution, terrainData.heightmapResolution));
      for (int y = 0; y < terrainData.heightmapResolution; ++y)
      {
        for (int x = 0; x < terrainData.heightmapResolution; ++x)
        {
          if (!(x == peak.x && y == peak.y))
          {
            float distToPeak = Vector2.Distance(peakLocation, new Vector2(x, y)) / maxDistance;
            float h = 0;
            switch (voronoiType)
            {
              case VoronoiType.Combined:
                {
                  h = peak.y - distToPeak * voronoiFallOff - Mathf.Pow(distToPeak, voronoiDropOff);
                  break;
                }
              case VoronoiType.Linear:
                {
                  h = peak.y - distToPeak * voronoiFallOff;
                  break;
                }
              case VoronoiType.Power:
                {
                  h = peak.y - Mathf.Pow(distToPeak, voronoiDropOff) * voronoiFallOff;
                  break;
                }
              case VoronoiType.SinPow:
                {
                  h = peak.y - Mathf.Pow(distToPeak * 3, voronoiFallOff) -
                    Mathf.Sin(distToPeak * 2 * Mathf.PI) / voronoiDropOff;
                  break;
                }
            };
            //float h = peak.y - Mathf.Sin(distToPeak * 100)*0.01f;
            if (heightMap[x, y] < h)
            {
              heightMap[x, y] = h;
            }
          }
        }
      }
    }

    terrainData.SetHeights(0, 0, heightMap);
  }

  public void Perline()
  {
    int height = terrainData.heightmapResolution;
    float[,] heightMap = GetHeightMap();

    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < height; ++x)
      {
        //heightMap[x, y] = Mathf.PerlinNoise((x * perlinFrequency)+perlinXOffset , (y * perlinAmplitude)+perlinYOffset);
        heightMap[x, y] += Utils.fBM
          ((x + perlinXOffset) * perlinAmplitude,
          (y + perlinYOffset) * perlinFrequency,
          perlinOctaves,
          perlinPersistance) * perlinHeightScale;
      }
    }
    terrainData.SetHeights(0, 0, heightMap);
  }

  public void MultiPerline()
  {
    int height = terrainData.heightmapResolution;
    float[,] heightMap = GetHeightMap();

    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < height; ++x)
      {
        foreach (PerlineParams current in perlineParams)
        {
          //heightMap[x, y] = Mathf.PerlinNoise((x * perlinFrequency)+perlinXOffset , (y * perlinAmplitude)+perlinYOffset);
          heightMap[x, y] += Utils.fBM
            ((x + current.XOffset) * current.XScale,
            (y + current.YOffset) * current.YScale,
            current.Octaves,
            current.Persistance) * current.HeightScale;
        }
      }
    }
    terrainData.SetHeights(0, 0, heightMap);
  }

  public void AddNewPerlin()
  {
    perlineParams.Add(new PerlineParams());
  }

  public void RemovePerlin()
  {
    List<PerlineParams> keptPrams = new List<PerlineParams>();
    for (int i = 0; i < perlineParams.Count; ++i)
    {
      if (!perlineParams[i].remove)
      {
        keptPrams.Add(perlineParams[i]);
      }
    }
    if (keptPrams.Count == 0)
    {
      keptPrams.Add(perlineParams[0]);
    }
    perlineParams = keptPrams;
  }

  public void RandomTerrain()
  {
    int height = terrainData.heightmapResolution;
    float[,] heightMap = GetHeightMap();
    for (int x = 0; x < height; ++x)
    {
      for (int z = 0; z < height; ++z)
      {
        heightMap[x, z] += UnityEngine.Random.Range(randomHeightRange.x, randomHeightRange.y);
      }
    }
    terrainData.SetHeights(0, 0, heightMap);

  }

  public void LoadTexture()
  {
    int height = terrainData.heightmapResolution;
    float[,] heightMap = GetHeightMap();
    for (int x = 0; x < height; ++x)
    {
      for (int y = 0; y < height; ++y)
      {
        // get pixel grayscale form image assign each vertex
        heightMap[x, y] += heightMapImage.GetPixel((int)(x * heightMapScale.x), (int)(y * heightMapScale.z)).grayscale * heightMapScale.y;
      }
    }
    terrainData.SetHeights(0, 0, heightMap);
  }

  public void ResetTerrain()
  {
    int height = terrainData.heightmapResolution;
    float[,] heightMap = new float[height, height];
    for (int x = 0; x < height; ++x)
    {
      for (int z = 0; z < height; ++z)
      {
        heightMap[x, z] = 0;
      }
    }
    terrainData.SetHeights(0, 0, heightMap);

  }

  void OnEnable()
  {
    Debug.Log("Initialising Terrain Data");
    terrain = GetComponent<Terrain>();
    terrainData = Terrain.activeTerrain.terrainData;
  }
  public enum TagType { Tag = 0, Layer = 1 }
  [SerializeField]
  int terrainLayer = -1;
  void Awake()
  {
    SerializedObject tagManager = new SerializedObject(
                          AssetDatabase.LoadAllAssetsAtPath("ProjectSettings/TagManager.asset")[0]);
    SerializedProperty tagsProp = tagManager.FindProperty("tags");

    AddTag(tagsProp, "Terrain", TagType.Tag);
    AddTag(tagsProp, "Cloud", TagType.Tag);
    AddTag(tagsProp, "Shore", TagType.Tag);

    //apply tag changes to tag database
    tagManager.ApplyModifiedProperties();

    SerializedProperty layerProp = tagManager.FindProperty("layers");
    AddTag(layerProp, "Sky", TagType.Layer);
    terrainLayer = AddTag(layerProp, "Terrain", TagType.Layer);
    tagManager.ApplyModifiedProperties();
    //take this object
    this.gameObject.tag = "Terrain";
    this.gameObject.layer = terrainLayer;
  }

  int AddTag(SerializedProperty tagsProp, string newTag, TagType tType)
  {
    bool found = false;
    //ensure the tag doesn't already exist
    for (int i = 0; i < tagsProp.arraySize; i++)
    {
      SerializedProperty t = tagsProp.GetArrayElementAtIndex(i);
      if (t.stringValue.Equals(newTag)) { found = true; return i; }
    }
    //add your new tag
    if (!found && tType == TagType.Tag)
    {
      tagsProp.InsertArrayElementAtIndex(0);
      SerializedProperty newTagProp = tagsProp.GetArrayElementAtIndex(0);
      newTagProp.stringValue = newTag;
    }
    else if (!found && tType == TagType.Layer)
    {
      for (int j = 9; j < tagsProp.arraySize; ++j)
      {
        SerializedProperty newLayer = tagsProp.GetArrayElementAtIndex((j));
        if (newLayer.stringValue == "")
        {
          newLayer.stringValue = newTag;
          return j;

        }
      }
    }
    return -1;
  }

  // Use this for initialization
  void Start()
  {

  }

  // Update is called once per frame
  void Update()
  {

  }
}
