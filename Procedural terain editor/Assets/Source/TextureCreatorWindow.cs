using UnityEditor;
using UnityEngine;
using System.IO;

public class TextureCreatorWindow : EditorWindow
{

  string filename = "myProceduralTexure";
  float perlinXscale;
  float perlinYscale;
  int perlinOctaves;
  float perlinPersistance;
  float perlinHeightScale;
  int perlinOffsetX;
  int perlinOffsetY;
  bool alphaToggle = false;
  bool seamlessToggle = false;
  bool mapToggle = false;

  float brightness = 0.5f;
  float contrast = 0.5f;

  Texture2D pTexure;
  [MenuItem("Window/TextureCreatorWindow")]
  public static void ShowWindow()
  {
    EditorWindow.GetWindow(typeof(TextureCreatorWindow));
  }

  private void OnEnable()
  {
    pTexure = new Texture2D(513, 513, TextureFormat.ARGB32, false);
  }

  private void OnGUI()
  {
    GUILayout.Label("Settings", EditorStyles.boldLabel);
    filename = EditorGUILayout.TextField("Texture Name", filename);

    int wSize = (int)(EditorGUIUtility.currentViewWidth - 100);

    perlinXscale = EditorGUILayout.Slider("X Scale", perlinXscale, 0, 0.1f);
    perlinYscale = EditorGUILayout.Slider("Y Scale", perlinYscale, 0, 0.1f);
    perlinOctaves = EditorGUILayout.IntSlider("Octaves", perlinOctaves, 1, 10);
    perlinPersistance = EditorGUILayout.Slider("Persistance", perlinPersistance, 1, 10);
    perlinHeightScale = EditorGUILayout.Slider("HeightScale", perlinHeightScale, 0, 1);
    perlinOffsetX = EditorGUILayout.IntSlider("Offset X", perlinOffsetX, 0, 10000);
    perlinOffsetY = EditorGUILayout.IntSlider("Offset Y", perlinOffsetY, 0, 10000);
    brightness = EditorGUILayout.Slider("Brightness", brightness, 0, 2);
    contrast = EditorGUILayout.Slider("Contrast", contrast, 0, 2);
    alphaToggle = EditorGUILayout.Toggle("Alpha?", alphaToggle);
    mapToggle = EditorGUILayout.Toggle("Map?", mapToggle);
    seamlessToggle = EditorGUILayout.Toggle("Seamless?", seamlessToggle);

    GUILayout.BeginHorizontal();
    GUILayout.FlexibleSpace();

    float minColor = 1;
    float maxColor = 0;

    if (GUILayout.Button("Generate", GUILayout.Width(wSize)))
    {
      int w = 513;
      int h = 513;
      float pValue;
      Color pixelColor = Color.white;
      for (int y = 0; y < h; ++y)
      {
        for (int x = 0; x < w; ++x)
        {
          if (seamlessToggle)
          {
            float u = (float)x / (float)w;
            float v = (float)y / (float)h;
            float noise00 = Utils.fBM((x + perlinOffsetX) * perlinXscale,
                                      (y + perlinOffsetY) * perlinYscale,
                                      perlinOctaves,
                                      perlinPersistance) * perlinHeightScale;
            float noise01 = Utils.fBM((x + perlinOffsetX) * perlinXscale,
                                      (y + perlinOffsetY + h) * perlinYscale,
                                      perlinOctaves,
                                      perlinPersistance) * perlinHeightScale;
            float noise10 = Utils.fBM((x + perlinOffsetX + w) * perlinXscale,
                                      (y + perlinOffsetY) * perlinYscale,
                                      perlinOctaves,
                                      perlinPersistance) * perlinHeightScale;
            float noise11 = Utils.fBM((x + perlinOffsetX + w) * perlinXscale,
                                      (y + perlinOffsetY + h) * perlinYscale,
                                      perlinOctaves,
                                      perlinPersistance) * perlinHeightScale;
            float noiseTotal = u * v * noise00 +
                               u * (1 - v) * noise01 +
                              (1 - u) * v * noise10 +
                              (1 - u) * (1 - v) * noise11;
            float value = (int)(256 * noiseTotal) + 50;
            float r = Mathf.Clamp((int)noise00, 0, 255);
            float g = Mathf.Clamp(value, 0, 255);
            float b = Mathf.Clamp(value + 50, 0, 255);
            float a = Mathf.Clamp(value + 100, 0, 255);
            pValue = (r + g + b) / (3 * 255.0f);
          }
          else
          {
            pValue = Utils.fBM((x + perlinOffsetX) * perlinXscale,
            (y + perlinOffsetY) * perlinYscale,
            perlinOctaves,
            perlinPersistance) * perlinHeightScale;
          }
          float colValue = contrast * (pValue - 0.5f)+0.5f * brightness;
          if (minColor > colValue)
          {
            minColor = colValue;
          }
          if (maxColor < colValue)
          {
            maxColor = colValue;
          }
          pixelColor = new Color(colValue, colValue, colValue, alphaToggle ? colValue : 1);
          pTexure.SetPixel(x, y, pixelColor);
        }
      }
      if (mapToggle)
      {
        for (int y = 0; y < h; ++y)
        {
          for (int x = 0; x < w; ++x)
          {
            pixelColor = pTexure.GetPixel(x, y);
            float colorValue = pixelColor.r;
            colorValue = Utils.Map(colorValue, minColor, maxColor, 0, 1);
            pixelColor.r = colorValue;
            pixelColor.g = colorValue;
            pixelColor.b = colorValue;
            pTexure.SetPixel(x,y,pixelColor);
          }
        }
      }
      pTexure.Apply(false, false);
    }
    GUILayout.FlexibleSpace();
    GUILayout.EndHorizontal();


    GUILayout.BeginHorizontal();
    GUILayout.FlexibleSpace();
    GUILayout.Label(pTexure, GUILayout.Width(wSize), GUILayout.Height(wSize));
    GUILayout.FlexibleSpace();
    GUILayout.EndHorizontal();

    GUILayout.BeginHorizontal();
    GUILayout.FlexibleSpace();
    if (GUILayout.Button("Save", GUILayout.Width(wSize)))
    {
      // Encode the texture as a PNG
      byte[] bytes = pTexure.EncodeToPNG();
      // Make sure that the destination folder exists
      const string parentFolder = "Assets";
      const string childFolder = "SavedTextures";
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
}
