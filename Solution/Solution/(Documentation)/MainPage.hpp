
/**
* \mainpage notitle
* \anchor MainPage
* 
*  Erosion
*  =======
*  
*  [Репозиторий](https://github.com/Unicornum/Research.Erosion) исходного кода.
*  
*  Представлены результаты обработки карт высот разного размера (512x512,
*  1024x1024 и 2048x2048) в двух вариантах (оттенки серого и цветная, в которой
*  цвет зависит от высоты на карте высот).
*  
*  Время указано для обработки карты высот 2048х2048.
*  
* \note
*  Все изображения кликабельны.
*  
*  Исходная карта высот, которую будем терзать
*  -------------------------------------------
* 
* \htmlonly

<img class="PreviewImage" src="Source_grayscale_2048x2048.png">
<img class="PreviewImage" src="Source_colors_2048x2048.png">

* \endhtmlonly
*
* \copydoc DommainDistortionPage
*  
* \copydoc ThermalErosionByAxelParisPage
*
* \copydoc TinyErodePage
*
* \copydoc GavoronoisePage
*
* \copydoc WaterErosionByEDogPage
*
* \copydoc HydraulicErosionByJobTallePage
* 
*  ... by David Jorna
*  ------------------
*  
*  [Источник](https://github.com/djorna/terrain-generation)
*  
* \copydoc ThermalErosionByDavidJornaPage
*
* \copydoc FastErosionByDavidJornaPage
*
* \copydoc MultiScaleErosionPage
*
* \copydoc TerrainErosionByAdrianBlumerPage
*  
*  ... by Nick McDonald
*  --------------------
*  
* \copydoc SimpleErosionByNickMcDonaldPage
*
* \copydoc SimpleHydrologyByNickMcDonaldPage
*
*  ----------------------------------------------------------------------------
* \copydoc PreviewPage
*/

/**
* \page ScreenShotsPage ScreenShots
*
*  Список файлов изображений, нужен для того, чтобы они попали в документацию.
* 
* \image html ".\Solution\Solution\(Documentation)\Source_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\Source_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\DommainDistortion_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\DommainDistortion_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\DommainDistortion_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\DommainDistortion_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\DommainDistortion_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\DommainDistortion_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParis_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParis_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParis_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParis_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParis_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParis_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\TinyErode_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\TinyErode_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\TinyErode_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\TinyErode_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\TinyErode_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\TinyErode_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\Gavoronoise_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\Gavoronoise_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\Gavoronoise_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\Gavoronoise_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\Gavoronoise_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\Gavoronoise_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParisGavoronoise_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParisGavoronoise_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParisGavoronoise_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParisGavoronoise_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParisGavoronoise_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByAxelParisGavoronoise_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\WaterErosionByEDog_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\WaterErosionByEDog_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\WaterErosionByEDog_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\WaterErosionByEDog_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\WaterErosionByEDog_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\WaterErosionByEDog_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByJobTalle_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByJobTalle_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByJobTalle_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByJobTalle_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByJobTalle_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByJobTalle_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByDavidJorna_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByDavidJorna_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByDavidJorna_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByDavidJorna_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByDavidJorna_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\ThermalErosionByDavidJorna_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\FastErosionByDavidJorna_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\FastErosionByDavidJorna_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\FastErosionByDavidJorna_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\FastErosionByDavidJorna_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\FastErosionByDavidJorna_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\FastErosionByDavidJorna_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByDavidJorna_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByDavidJorna_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByDavidJorna_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByDavidJorna_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByDavidJorna_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\HydraulicErosionByDavidJorna_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\MultiScaleErosion_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\MultiScaleErosion_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\MultiScaleErosion_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\MultiScaleErosion_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\MultiScaleErosion_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\MultiScaleErosion_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\TerrainErosionByAdrianBlumer_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\TerrainErosionByAdrianBlumer_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\TerrainErosionByAdrianBlumer_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\TerrainErosionByAdrianBlumer_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\TerrainErosionByAdrianBlumer_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\TerrainErosionByAdrianBlumer_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleErosionByNickMcDonald_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleErosionByNickMcDonald_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleErosionByNickMcDonald_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleErosionByNickMcDonald_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleErosionByNickMcDonald_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleErosionByNickMcDonald_colors_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleHydrologyByNickMcDonald_grayscale_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleHydrologyByNickMcDonald_colors_512x512.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleHydrologyByNickMcDonald_grayscale_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleHydrologyByNickMcDonald_colors_1024x1024.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleHydrologyByNickMcDonald_grayscale_2048x2048.png"
* \image html ".\Solution\Solution\(Documentation)\SimpleHydrologyByNickMcDonald_colors_2048x2048.png"
*/

/**
* \page PreviewPage
* \htmlonly

<style>

.PreviewImage {
  width:100%;
  max-width:160px;
  border-radius: 10px;
  cursor: pointer;
  transition: 0.3s;
}

.PreviewImage:hover {
  opacity: 0.7;
}

.modal{
  display: none;
  position: fixed;
  z-index: 9999;
  padding-top: 100px;
  left : 0;
  top : 0;
  width: 100%;
  height: 100%;
  max-width: 100%;
  max-height: 100%;
  overflow: auto;
  background-color: rgb(0,0,0);
  background-color: rgba(0,0,0,0.9);
}

.modal-content{
  margin: auto;
  display: block;
  height: 80%;
}

#caption{
  margin: auto;
  display: block;
  width: 80%;
  max-width: 700px;
  text-align: center;
  color: #ccc;
  padding: 10px 0;
  height: 150px;
}

.modal-content, #caption{
  animation-name: zoom;
  animation-duration: 0.6s;
}

@keyframes zoom{
  from {transform:scale(0)}
  to {transform:scale(1)}
}

.close{
  position: absolute;
  top: 15px;
  right: 35px;
  color: #f1f1f1;
  font-size: 40px;
  font-weight: bold;
  transition: 0.3s;
}

.close:hover,
.close:focus{
  color: #bbb;
  text-decoration: none;
  cursor: pointer;
  font-size: 50px;
}

@media only screen and (max-width: 700px) {
  .modal-content{
    width: 100 % ;
  }
}
</style>

<!-- Окно предварительного просмотра -->
<div id="PreviewWindow" class="modal">
  <span class="close">&times;</span>
  <img class="modal-content" id="FullSizeImage">
</div>

<script>

// Для всех изображений...
var Images = document.getElementsByClassName('PreviewImage');

for (i = 0; i < Images.length; i++)
{
  Images[i].onclick = function()
  {
    document.getElementById('PreviewWindow').style.display = "block";
    document.getElementById("FullSizeImage").src = this.src;
  };
}

// Для элемента, закрывающего окно предпросмотра...
var Close = document.getElementsByClassName("close")[0];

Close.onclick = function()
{
  document.getElementById('PreviewWindow').style.display = "none";
}

</script>

* \endhtmlonly
*/
