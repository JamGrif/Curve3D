using System.Diagnostics;

namespace Launcher
{
    public partial class LauncherForm : Form
    {
        private const int NO_SCENE_SELECTED = -1;
        private const string PROCESS_NAME = "Curve3D";

        // All selectable scenes 
        private List<string> AllScenesList = new List<string>();

        // Scene selected in ListBox
        private string SelectedScene = string.Empty;

        private Process Curve3DProcess = null;

        public LauncherForm()
        {
            InitializeComponent();
        }

        private void LauncherForm_FormClosing(object sender, EventArgs e)
        {
            // Close Curve3D
            Process[] processList = Process.GetProcessesByName(PROCESS_NAME);
            if (processList.Length > 0)
                Curve3DProcess.CloseMainWindow();
        }

        private void LauncherForm_Load(object sender, EventArgs e)
        {
            // Only used when developing on own machine, ignored if ran from another users machine
            string BatchfilePath = "../../../RefreshResFile.bat";
            if (File.Exists(BatchfilePath))
            {
                Process? BatchfileProcess = Process.Start(BatchfilePath);
                BatchfileProcess.WaitForExit();
            }

            // Get all loadable scenes
            string[] AllSceneFiles = System.IO.Directory.GetFiles(@"res\scenes", "*.xml");
            foreach (string file in AllSceneFiles)
                AllScenesList.Add(Path.GetFileNameWithoutExtension(file));

            // Stops first item in ListBox from being selected by default
            SelectSceneListBox.SelectedIndex = NO_SCENE_SELECTED;
            SelectSceneListBox.DataSource = AllScenesList;
        }

        // Launch button - Start the Curve3D.exe process
        private void LaunchButton_Click(object sender, EventArgs e)
        {
            if (SelectedScene == string.Empty)
                return;

            StartProcess(SelectedScene);
        }

        // ListBox - Store selected scene
        private void SceneSelectListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (SelectSceneListBox.SelectedIndex == NO_SCENE_SELECTED)
                return;

            SelectedScene = AllScenesList[SelectSceneListBox.SelectedIndex];
        }

        // Start Curve3D.exe with the startSceneName scene
        private void StartProcess(string startSceneName)
        {
            // Ensure Curve3D is not already running
            Process[] processList = Process.GetProcessesByName(PROCESS_NAME);
            if (processList.Length > 0)
            {
                MessageBox.Show("An instance of Curve3D is already running!", "Curve3D Launcher", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            Curve3DProcess = Process.Start(new ProcessStartInfo()
            {
                FileName = PROCESS_NAME,
                RedirectStandardOutput = false, // Coloured text in Curve3D console
                UseShellExecute = true,
                ArgumentList = { startSceneName },
            });
        }

        private void GitHub_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start(new ProcessStartInfo("https://github.com/JamGrif") { UseShellExecute = true });
        }

        private void YouTube_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start(new ProcessStartInfo("https://www.youtube.com/@JamGrif") { UseShellExecute = true });
        }
    }
}
