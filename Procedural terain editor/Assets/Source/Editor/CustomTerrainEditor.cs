using UnityEngine;
using UnityEditor;
using EditorGUITable;
using System.IO;
using static CustomTerrain;

[CustomEditor(typeof(CustomTerrain))]
[CanEditMultipleObjects]

public class CustomTerrainEditor : Editor
{

  //properties -----------
  SerializedProperty randomHeightRange;
  SerializedProperty heightMapScale;
  SerializedProperty heightMapImage;
  SerializedProperty perlinFrequency;
  SerializedProperty perlinAmplitude;
  SerializedProperty perlinXOffset;
  SerializedProperty perlinYOffset;
  SerializedProperty perlinOctaves;
  SerializedProperty perlinPersistance;
  SerializedProperty perlinHeightScale;
  SerializedProperty resetTerrain;

  SerializedProperty voronoiFallOff;
  SerializedProperty voronoiDropOff;
  SerializedProperty voronoiMinHeight;
  SerializedProperty voronoiMaxHeight;
  SerializedProperty voronoiPeaks;
  SerializedProperty voronoiType;

  SerializedProperty MPDheightMin;
  SerializedProperty MPDheightMax;
  SerializedProperty MPDheightDampenerPower;
  SerializedProperty MPDroughness;
  SerializedProperty SmoothRepeat;

  SerializedProperty terrianOffset;
  SerializedProperty terrainNoiseX;
  SerializedProperty terrainNoiseY;
  SerializedProperty terrainNoiseScaler;

  GUITableState vegMapTable;
  SerializedProperty vegetation;
  SerializedProperty maxTrees;
  SerializedProperty treeSpacing;

  SerializedProperty perlinParams;
  GUITableState perlinPramsTable;

  SerializedProperty terrainHeights;
  GUITableState terrainLayersTable;

  SerializedProperty waterHeight;
  SerializedProperty waterGO;
  SerializedProperty shoreLineMaterial;

  Texture2D hmTexture;
  string filename = "Saved HeightMap";


  GUITableState detailMapTable;
  SerializedProperty detail;
  SerializedProperty maxDetails;
  SerializedProperty detailSpacing;

  SerializedProperty erosionType;
  SerializedProperty ErosionStrength;
  SerializedProperty riversPerDroplet;
  SerializedProperty solubility;
  SerializedProperty droplets;
  SerializedProperty erosionSmoothAmount;
  SerializedProperty thermalStrength;
  SerializedProperty windDirection;


  SerializedProperty numClouds;
  SerializedProperty particlesPerCloud;
  SerializedProperty cloudMaterial;
  SerializedProperty cloudShadowMaterial;
  SerializedProperty cloudStartSize;
  SerializedProperty cloudColor;
  SerializedProperty cloudLining;
  SerializedProperty cloudMinSpeed;
  SerializedProperty cloudMaxSpeed;
  SerializedProperty cloudRange;
  SerializedProperty cloudScaleMin;
  SerializedProperty cloudScaleMax;





  //fold outs ------------
  bool showRandom = false;
  bool showLoadHeights = false;
  bool showPerlinNoise = false;
  bool showMutliPerlin = false;
  bool showVoronoi = false;
  bool showMPD = false;
  bool showSmooth = false;
  bool showTerrainLayers = false;
  bool showHeights = false;
  bool showVeg = false;
  bool showDetail = false;
  bool showWater = false;
  bool showErosion = false;
  bool showClouds = false;
  void OnEnable()
  {
    randomHeightRange = serializedObject.FindProperty("randomHeightRange");
    heightMapScale = serializedObject.FindProperty("heightMapScale");
    heightMapImage = serializedObject.FindProperty("heightMapImage");
    perlinFrequency = serializedObject.FindProperty("perlinFrequency");
    perlinAmplitude = serializedObject.FindProperty("perlinAmplitude");
    perlinXOffset = serializedObject.FindProperty("perlinXOffset");
    perlinYOffset = serializedObject.FindProperty("perlinYOffset");
    perlinOctaves = serializedObject.FindProperty("perlinOctaves");
    perlinPersistance = serializedObject.FindProperty("perlinPersistance");
    perlinHeightScale = serializedObject.FindProperty("perlinHeightScale");
    resetTerrain = serializedObject.FindProperty("resetTerrain");
    perlinPramsTable = new GUITableState("Perlin Paramaters");
    perlinParams = serializedObject.FindProperty("perlinParams");

    voronoiFallOff = serializedObject.FindProperty("voronoiFallOff");
    voronoiDropOff = serializedObject.FindProperty("voronoiDropOff");
    voronoiMinHeight = serializedObject.FindProperty("voronoiMinHeight");
    voronoiMaxHeight = serializedObject.FindProperty("voronoiMaxHeight");
    voronoiPeaks = serializedObject.FindProperty("voronoiPeaks");
    voronoiType = serializedObject.FindProperty("voronoiType");

    MPDheightMin = serializedObject.FindProperty("MPDheightMin");
    MPDheightMax = serializedObject.FindProperty("MPDheightMax");
    MPDheightDampenerPower = serializedObject.FindProperty("MPDheightDampenerPower");
    MPDroughness = serializedObject.FindProperty("MPDroughness");
    SmoothRepeat = serializedObject.FindProperty("SmoothRepeat");

    terrainHeights = serializedObject.FindProperty("terrainHeights");
    terrainLayersTable = new GUITableState("Terrain Layers");
    terrianOffset = serializedObject.FindProperty("terrianOffset");
    terrainNoiseX = serializedObject.FindProperty("terrainNoiseX");
    terrainNoiseY = serializedObject.FindProperty("terrainNoiseY");
    terrainNoiseScaler = serializedObject.FindProperty("terrainNoiseScaler");

    vegMapTable = new GUITableState("vegMapTable");
    vegetation = serializedObject.FindProperty("vegetation");
    maxTrees = serializedObject.FindProperty("maxTrees");
    treeSpacing = serializedObject.FindProperty("treeSpacing");

    hmTexture = new Texture2D(513, 513, TextureFormat.ARGB32, false);

    detailMapTable = new GUITableState("detailMapTable");
    detail = serializedObject.FindProperty("details");
    maxDetails = serializedObject.FindProperty("maxDetails");
    detailSpacing = serializedObject.FindProperty("detailSpacing");

    waterHeight = serializedObject.FindProperty("waterHeight");
    waterGO = serializedObject.FindProperty("waterGO");
    shoreLineMaterial = serializedObject.FindProperty("shoreLineMaterial");

    erosionType = serializedObject.FindProperty("erosionType");
    ErosionStrength = serializedObject.FindProperty("erosionStrength");
    riversPerDroplet = serializedObject.FindProperty("riversPerDroplet");
    solubility = serializedObject.FindProperty("solubility");
    droplets = serializedObject.FindProperty("droplets");
    erosionSmoothAmount = serializedObject.FindProperty("erosionSmoothAmount");
    thermalStrength = serializedObject.FindProperty("thermalStrength");
    windDirection = serializedObject.FindProperty("windDirection");

    numClouds = serializedObject.FindProperty("numClouds");
    particlesPerCloud = serializedObject.FindProperty("particlesPerCloud");
    cloudMaterial = serializedObject.FindProperty("cloudMaterial");
    cloudShadowMaterial = serializedObject.FindProperty("cloudShadowMaterial");
    cloudStartSize = serializedObject.FindProperty("cloudStartSize");
    cloudColor = serializedObject.FindProperty("cloudColor");
    cloudLining = serializedObject.FindProperty("cloudLining");
    cloudMinSpeed = serializedObject.FindProperty("cloudMinSpeed");
    cloudMaxSpeed = serializedObject.FindProperty("cloudMaxSpeed");
    cloudRange = serializedObject.FindProperty("cloudRange");
    cloudScaleMin = serializedObject.FindProperty("cloudScaleMin");
    cloudScaleMax = serializedObject.FindProperty("cloudScaleMax");

  }

  public override void OnInspectorGUI()
  {
    serializedObject.Update();

    CustomTerrain terrain = (CustomTerrain)target;
    EditorGUILayout.PropertyField(resetTerrain);

    showRandom = EditorGUILayout.Foldout(showRandom, "Random");
    if (showRandom)
    {
      EditorGUILayout.LabelField("", GUI.skin.horizontalSlider);
      GUILayout.Label("Set Heights Between Random Values", EditorStyles.boldLabel);
      EditorGUILayout.PropertyField(randomHeightRange);
      if (GUILayout.Button("Random Heights"))
      {
        terrain.RandomTerrain();
      }
    }

    showLoadHeights = EditorGUILayout.Foldout(showLoadHeights, "Load Heights");
    if (showLoadHeights)
    {
      EditorGUILayout.LabelField("", GUI.skin.horizontalSlider);
      GUILayout.Label("Load Heights From Texture", EditorStyles.boldLabel);
      EditorGUILayout.PropertyField(heightMapImage);
      EditorGUILayout.PropertyField(heightMapScale);
      if (GUILayout.Button("Load Texture"))
      {
        terrain.LoadTexture();
      }
    }

    showPerlinNoise = EditorGUILayout.Foldout(showPerlinNoise, "Perlin Noise");
    if (showPerlinNoise)
    {
      EditorGUILayout.LabelField("", GUI.skin.horizontalSlider);
      GUILayout.Label("Load Heights From Texture", EditorStyles.boldLabel);
      EditorGUILayout.Slider(perlinAmplitude, 0, 0.001f, new GUIContent("X Scale"));
      EditorGUILayout.Slider(perlinFrequency, 0, 0.001f, new GUIContent("Y Scale"));
      //maybe we can generate after slider updates
      EditorGUILayout.Slider(perlinXOffset, 0, 10000, new GUIContent("X Offset"));
      EditorGUILayout.Slider(perlinYOffset, 0, 10000, new GUIContent("Y Offset"));
      EditorGUILayout.IntSlider(perlinOctaves, 1, 10, new GUIContent("Octaves"));
      EditorGUILayout.Slider(perlinPersistance, 0.1f, 10, new GUIContent("Persistance"));
      EditorGUILayout.Slider(perlinHeightScale, 0, 1, new GUIContent("Height Scale"));


      if (GUILayout.Button("Generate Perline Noise"))
      {
        terrain.Perline();
      }
    }

    showMutliPerlin = EditorGUILayout.Foldout(showMutliPerlin, "Multiple Perlin Noise");
    if (showMutliPerlin)
    {
      EditorGUILayout.LabelField("", GUI.skin.horizontalSlider);
      GUILayout.Label("Multiple Perlin Noise", EditorStyles.boldLabel);
      perlinPramsTable = GUITableLayout.DrawTable(perlinPramsTable, serializedObject.FindProperty("perlineParams"));
      GUILayout.Space(20);
      EditorGUILayout.BeginHorizontal();
      if (GUILayout.Button("+"))
      {
        terrain.AddNewPerlin();
      }
      if (GUILayout.Button("-"))
      {
        terrain.RemovePerlin();
      }
      EditorGUILayout.EndHorizontal();
      if (GUILayout.Button("Generate Multiple Perlin Noise"))
      {
        terrain.MultiPerline();
      }

    }
    showVoronoi = EditorGUILayout.Foldout(showVoronoi, "Voronoi");
    if (showVoronoi)
    {
      EditorGUILayout.IntSlider(voronoiPeaks, 1, 30, new GUIContent("Peaks"));
      EditorGUILayout.Slider(voronoiFallOff, 0, 10, new GUIContent("Fall Off"));
      EditorGUILayout.Slider(voronoiDropOff, 0, 10, new GUIContent("Drop Off"));
      EditorGUILayout.Slider(voronoiMinHeight, 0, 1, new GUIContent("Min Height"));
      EditorGUILayout.Slider(voronoiMaxHeight, 0, 1, new GUIContent("Max Height"));
      EditorGUILayout.PropertyField(voronoiType);
      if (GUILayout.Button("Voronoi"))
      {
        terrain.Voronoi();
      }
    }

    showMPD = EditorGUILayout.Foldout(showMPD, "Mid Point Displacment");
    if (showMPD)
    {
      EditorGUILayout.PropertyField(MPDheightMin);
      EditorGUILayout.PropertyField(MPDheightMax);
      EditorGUILayout.PropertyField(MPDheightDampenerPower);
      EditorGUILayout.PropertyField(MPDroughness);
      if (GUILayout.Button("Mid Point Displacment"))
      {
        terrain.MPD();
      }
    }

    showTerrainLayers = EditorGUILayout.Foldout(showTerrainLayers, "Terrain Layers");
    if (showTerrainLayers)
    {
      EditorGUILayout.LabelField("", GUI.skin.horizontalSlider);
      GUILayout.Label("Terrain Layers", EditorStyles.boldLabel);

      EditorGUILayout.Slider(terrianOffset, 0, 0.1f, new GUIContent("Offset"));
      EditorGUILayout.Slider(terrainNoiseX, 0.001f, 1, new GUIContent("Noise X"));
      EditorGUILayout.Slider(terrainNoiseY, 0.001f, 1, new GUIContent("Noise Y"));
      EditorGUILayout.Slider(terrainNoiseScaler, 0, 1, new GUIContent("Noise Scaler"));

      terrainLayersTable = GUITableLayout.DrawTable(terrainLayersTable, serializedObject.FindProperty("terrainHeights"));
      GUILayout.Space(20);
      EditorGUILayout.BeginHorizontal();
      if (GUILayout.Button("+"))
      {
        terrain.AddTerrainLayer();
      }
      if (GUILayout.Button("-"))
      {
        terrain.RemoveTerrainLayer();
      }
      EditorGUILayout.EndHorizontal();
      if (GUILayout.Button("Apply Terrain Layers"))
      {
        terrain.TerrainMaps();
      }

    }

    showVeg = EditorGUILayout.Foldout(showVeg, "Vegetation");
    if (showVeg)
    {
      EditorGUILayout.LabelField("", GUI.skin.horizontalSlider);
      GUILayout.Label("Vegetation", EditorStyles.boldLabel);
      EditorGUILayout.IntSlider(maxTrees, 0, 10000, new GUIContent("Maximum Trees"));
      EditorGUILayout.IntSlider(treeSpacing, 2, 20, new GUIContent("Trees Spacing"));
      vegMapTable = GUITableLayout.DrawTable(vegMapTable,
                                  serializedObject.FindProperty("vegetation"));
      GUILayout.Space(20);
      EditorGUILayout.BeginHorizontal();
      if (GUILayout.Button("+"))
      {
        terrain.AddNewVegetation();
      }
      if (GUILayout.Button("-"))
      {
        terrain.RemoveVegetation();
      }
      EditorGUILayout.EndHorizontal();
      if (GUILayout.Button("Apply Vegetation"))
      {
        terrain.PlantVegetation();
      }
    }

    showDetail = EditorGUILayout.Foldout(showDetail, "Details");
    if (showDetail)
    {
      EditorGUILayout.LabelField("", GUI.skin.horizontalSlider);
      GUILayout.Label("Detail", EditorStyles.boldLabel);
      EditorGUILayout.IntSlider(maxDetails, 0, 10000, new GUIContent("Details View Distance"));
      EditorGUILayout.IntSlider(detailSpacing, 1, 20, new GUIContent("Detail Spacing"));
      detailMapTable = GUITableLayout.DrawTable(detailMapTable,
          serializedObject.FindProperty("details"));

      terrain.GetComponent<Terrain>().detailObjectDistance = maxDetails.intValue;

      GUILayout.Space(20);

      EditorGUILayout.BeginHorizontal();

      if (GUILayout.Button("+"))
      {
        terrain.AddNewDetails();
      }
      if (GUILayout.Button("-"))
      {
        terrain.RemoveDetails();
      }

      EditorGUILayout.EndHorizontal();
      if (GUILayout.Button("Apply Details"))
      {
        terrain.AddDetails();
      }
    }
    showWater = EditorGUILayout.Foldout(showWater, "Water");
    if (showWater)
    {
      EditorGUILayout.LabelField("", GUI.skin.horizontalSlider);
      GUILayout.Label("Water", EditorStyles.boldLabel);
      EditorGUILayout.Slider(waterHeight, 0, 1.0f, new GUIContent("Water Height"));
      EditorGUILayout.PropertyField(waterGO);
      if (GUILayout.Button("Add Water"))
      {
        terrain.AddWater();
      }
      EditorGUILayout.PropertyField(shoreLineMaterial);
      if (GUILayout.Button("ShoreLine"))
      {
        terrain.DrawShoreLine();
      }
    }

    showErosion = EditorGUILayout.Foldout(showErosion, "Erosion");
    if (showErosion)
    {
      EditorGUILayout.PropertyField(erosionType);
      EditorGUILayout.Slider(ErosionStrength, 0, 1, new GUIContent("Erosion Strength"));
      EditorGUILayout.Slider(thermalStrength, 0.00001f, 1, new GUIContent("Thermal Strength"));
      EditorGUILayout.IntSlider(droplets, 0, 500, new GUIContent("Droplets"));
      EditorGUILayout.Slider(solubility, 0.001f, 1, new GUIContent("Solubility"));
      EditorGUILayout.IntSlider(riversPerDroplet, 1, 20, new GUIContent("Rivers Per Droplet"));
      EditorGUILayout.Slider(windDirection, 0, 360, new GUIContent("Wind Direction"));
      EditorGUILayout.IntSlider(erosionSmoothAmount, 0, 10, new GUIContent("Erosion Smooth Amount"));
      if (GUILayout.Button("Erode"))
      {
        terrain.Erode();
      }

    }

    showClouds = EditorGUILayout.Foldout(showClouds, "Clouds");
    if (showClouds)
    {
      EditorGUILayout.PropertyField(numClouds, new GUIContent("Number of Coulds"));
      EditorGUILayout.PropertyField(particlesPerCloud, new GUIContent("Particles Per Cloud"));
      EditorGUILayout.PropertyField(cloudStartSize, new GUIContent("Cloud Start Size"));
      EditorGUILayout.PropertyField(cloudScaleMin, new GUIContent("ScaleMin"));
      EditorGUILayout.PropertyField(cloudScaleMax, new GUIContent("Scale Max"));
      EditorGUILayout.PropertyField(cloudMaterial, true);
      EditorGUILayout.PropertyField(cloudShadowMaterial, true);
      EditorGUILayout.PropertyField(cloudColor, new GUIContent("Color"));
      EditorGUILayout.PropertyField(cloudLining, new GUIContent("Lining"));
      EditorGUILayout.PropertyField(cloudMinSpeed, new GUIContent("Min Speed"));
      EditorGUILayout.PropertyField(cloudMaxSpeed, new GUIContent("Max Speed"));
      EditorGUILayout.PropertyField(cloudRange, new GUIContent("Distanced Travelld"));
      if (GUILayout.Button("Generate Clouds"))
      {
        terrain.GenerateClouds();
      }

    }

    showSmooth = EditorGUILayout.Foldout(showSmooth, "Smooth");
    if (showSmooth)
    {
      EditorGUILayout.IntSlider(SmoothRepeat, 1, 25, new GUIContent("Repeat Times"));
      if (GUILayout.Button("Smooth"))
      {
        terrain.Smooth();
      }

    }
    EditorGUILayout.LabelField("", GUI.skin.horizontalSlider);
    if (GUILayout.Button("Reset Terrain"))
    {
      terrain.ResetTerrain();
    }

    showHeights = EditorGUILayout.Foldout(showHeights, "Heights");
    if (showHeights)
    {
      GUILayout.BeginHorizontal();
      GUILayout.FlexibleSpace();
      int hmtSize = (int)(EditorGUIUtility.currentViewWidth - 100);
      GUILayout.Label(hmTexture, GUILayout.Width(hmtSize), GUILayout.Height(hmtSize));
      GUILayout.FlexibleSpace();
      GUILayout.EndHorizontal();
      GUILayout.BeginHorizontal();
      GUILayout.FlexibleSpace();
      if (GUILayout.Button("Refresh", GUILayout.Width(hmtSize)))
      {
        float[,] heightMap = terrain.terrainData.GetHeights(0, 0, terrain.terrainData.heightmapResolution, terrain.terrainData.heightmapResolution);

        for (int y = 0; y < terrain.terrainData.alphamapHeight; ++y)
        {
          for (int x = 0; x < terrain.terrainData.alphamapWidth; ++x)
          {
            hmTexture.SetPixel(x, y, new Color(heightMap[x, y], heightMap[x, y], heightMap[x, y], 1));
          }
        }
        hmTexture.Apply();
      }
      GUILayout.FlexibleSpace();
      GUILayout.EndHorizontal();

      filename = EditorGUILayout.TextField("Texture Name", filename);
      GUILayout.BeginHorizontal();
      GUILayout.FlexibleSpace();
      if (GUILayout.Button("Save", GUILayout.Width(hmtSize)))
      {
        // Encode the texture as a PNG
        byte[] bytes = hmTexture.EncodeToPNG();
        // Make sure that the destination folder exists
        const string parentFolder = "Assets";
        const string childFolder = "SavedHeightMaps";
        string folderPath = $"{parentFolder}/{childFolder}";
        if (!AssetDatabase.IsValidFolder(folderPath))
        {
          AssetDatabase.CreateFolder(parentFolder, childFolder);
        }
        // Define the Texture's path in the AAssetDatabase
        string filePath = $"{folderPath}/{filename}.png";
        // Write the file to disk
        File.WriteAllBytes(filePath, bytes);
        // Refresh the AssetDatabase
        AssetDatabase.Refresh();
        // Get a reference to the TextureImporter associated to the png that was just saved
        var textureAsset = (TextureImporter)AssetImporter.GetAtPath(filePath);
        // Make it readable
        textureAsset.isReadable = true;
        // Update the AssetDatabase
        AssetDatabase.ImportAsset(filePath, ImportAssetOptions.ForceUpdate);
      }
      GUILayout.FlexibleSpace();
      GUILayout.EndHorizontal();

    }

    serializedObject.ApplyModifiedProperties();
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
