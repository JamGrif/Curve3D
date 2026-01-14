namespace Launcher
{
    partial class LauncherForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LauncherForm));
            LaunchButton = new Button();
            SelectSceneListBox = new ListBox();
            TEXT_Curve3D = new Label();
            TEXT_SelectScene = new Label();
            GitHub = new LinkLabel();
            YouTube = new LinkLabel();
            TEXT_byJamGrif = new Label();
            SuspendLayout();
            // 
            // LaunchButton
            // 
            LaunchButton.Font = new Font("Segoe UI", 12F);
            LaunchButton.Location = new Point(143, 405);
            LaunchButton.Margin = new Padding(3, 4, 3, 4);
            LaunchButton.Name = "LaunchButton";
            LaunchButton.Size = new Size(175, 95);
            LaunchButton.TabIndex = 0;
            LaunchButton.TabStop = false;
            LaunchButton.Text = "Launch";
            LaunchButton.UseVisualStyleBackColor = true;
            LaunchButton.Click += LaunchButton_Click;
            // 
            // SelectSceneListBox
            // 
            SelectSceneListBox.FormattingEnabled = true;
            SelectSceneListBox.Location = new Point(143, 148);
            SelectSceneListBox.Margin = new Padding(3, 4, 3, 4);
            SelectSceneListBox.Name = "SelectSceneListBox";
            SelectSceneListBox.Size = new Size(174, 244);
            SelectSceneListBox.TabIndex = 1;
            SelectSceneListBox.SelectedIndexChanged += SceneSelectListBox_SelectedIndexChanged;
            // 
            // TEXT_Curve3D
            // 
            TEXT_Curve3D.AutoSize = true;
            TEXT_Curve3D.Font = new Font("Segoe UI", 21.75F, FontStyle.Bold);
            TEXT_Curve3D.Location = new Point(155, 11);
            TEXT_Curve3D.Name = "TEXT_Curve3D";
            TEXT_Curve3D.Size = new Size(171, 50);
            TEXT_Curve3D.TabIndex = 3;
            TEXT_Curve3D.Text = "Curve3D";
            // 
            // TEXT_SelectScene
            // 
            TEXT_SelectScene.AutoSize = true;
            TEXT_SelectScene.Font = new Font("Segoe UI", 11.25F, FontStyle.Bold);
            TEXT_SelectScene.Location = new Point(167, 121);
            TEXT_SelectScene.Name = "TEXT_SelectScene";
            TEXT_SelectScene.Size = new Size(126, 25);
            TEXT_SelectScene.TabIndex = 4;
            TEXT_SelectScene.Text = "Select Scene:";
            // 
            // GitHub
            // 
            GitHub.AutoSize = true;
            GitHub.Font = new Font("Segoe UI", 12F, FontStyle.Bold);
            GitHub.Location = new Point(91, 509);
            GitHub.Name = "GitHub";
            GitHub.Size = new Size(79, 28);
            GitHub.TabIndex = 5;
            GitHub.TabStop = true;
            GitHub.Text = "GitHub";
            GitHub.LinkClicked += GitHub_LinkClicked;
            // 
            // YouTube
            // 
            YouTube.AutoSize = true;
            YouTube.Font = new Font("Segoe UI", 12F, FontStyle.Bold);
            YouTube.Location = new Point(254, 509);
            YouTube.Name = "YouTube";
            YouTube.Size = new Size(92, 28);
            YouTube.TabIndex = 6;
            YouTube.TabStop = true;
            YouTube.Text = "YouTube";
            YouTube.LinkClicked += YouTube_LinkClicked;
            // 
            // TEXT_byJamGrif
            // 
            TEXT_byJamGrif.AutoSize = true;
            TEXT_byJamGrif.Font = new Font("Segoe UI", 14.25F, FontStyle.Bold);
            TEXT_byJamGrif.Location = new Point(169, 60);
            TEXT_byJamGrif.Name = "TEXT_byJamGrif";
            TEXT_byJamGrif.Size = new Size(138, 32);
            TEXT_byJamGrif.TabIndex = 7;
            TEXT_byJamGrif.Text = "by JamGrif";
            // 
            // LauncherForm
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(496, 548);
            Controls.Add(TEXT_byJamGrif);
            Controls.Add(YouTube);
            Controls.Add(GitHub);
            Controls.Add(TEXT_SelectScene);
            Controls.Add(TEXT_Curve3D);
            Controls.Add(SelectSceneListBox);
            Controls.Add(LaunchButton);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Icon = (Icon)resources.GetObject("$this.Icon");
            Margin = new Padding(3, 4, 3, 4);
            MaximizeBox = false;
            Name = "LauncherForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Curve3D Launcher";
            FormClosing += LauncherForm_FormClosing;
            Load += LauncherForm_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button LaunchButton;
        private ListBox SelectSceneListBox;
        private Label TEXT_Curve3D;
        private Label TEXT_SelectScene;
        private LinkLabel GitHub;
        private LinkLabel YouTube;
        private Label TEXT_byJamGrif;
    }
}
