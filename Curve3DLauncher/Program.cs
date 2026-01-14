using System.Runtime.InteropServices;

namespace Launcher
{
    internal static class Program
    {
        [STAThread]
        static void Main()
        {
            ApplicationConfiguration.Initialize();
            Application.Run(new LauncherForm());
        }
    }
}
