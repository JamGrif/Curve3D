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
            SceneSelectListBox = new ListBox();
            label1 = new Label();
            label2 = new Label();
            GitHub = new LinkLabel();
            YouTube = new LinkLabel();
            label3 = new Label();
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
            LaunchButton.Text = "Launch Demo";
            LaunchButton.UseVisualStyleBackColor = true;
            LaunchButton.Click += LaunchButton_Click;
            // 
            // SceneSelectListBox
            // 
            SceneSelectListBox.FormattingEnabled = true;
            SceneSelectListBox.Location = new Point(143, 148);
            SceneSelectListBox.Margin = new Padding(3, 4, 3, 4);
            SceneSelectListBox.Name = "SceneSelectListBox";
            SceneSelectListBox.Size = new Size(174, 244);
            SceneSelectListBox.TabIndex = 1;
            SceneSelectListBox.SelectedIndexChanged += SceneSelectListBox_SelectedIndexChanged;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Font = new Font("Segoe UI", 21.75F, FontStyle.Bold);
            label1.Location = new Point(26, 11);
            label1.Name = "label1";
            label1.Size = new Size(487, 50);
            label1.TabIndex = 3;
            label1.Text = "OpenGL Project - Launcher";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new Font("Segoe UI", 11.25F, FontStyle.Bold);
            label2.Location = new Point(142, 112);
            label2.Name = "label2";
            label2.Size = new Size(191, 25);
            label2.TabIndex = 4;
            label2.Text = "Select scene to load:";
            // 
            // GitHub
            // 
            GitHub.AutoSize = true;
            GitHub.Font = new Font("Segoe UI", 12F, FontStyle.Bold);
            GitHub.Location = new Point(91, 509);
            GitHub.Name = "GitHub";
            GitHub.Size = new Size(115, 28);
            GitHub.TabIndex = 5;
            GitHub.TabStop = true;
            GitHub.Text = "My GitHub";
            GitHub.LinkClicked += GitHub_LinkClicked;
            // 
            // YouTube
            // 
            YouTube.AutoSize = true;
            YouTube.Font = new Font("Segoe UI", 12F, FontStyle.Bold);
            YouTube.Location = new Point(254, 509);
            YouTube.Name = "YouTube";
            YouTube.Size = new Size(128, 28);
            YouTube.TabIndex = 6;
            YouTube.TabStop = true;
            YouTube.Text = "My YouTube";
            YouTube.LinkClicked += YouTube_LinkClicked;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Font = new Font("Segoe UI", 14.25F, FontStyle.Bold);
            label3.Location = new Point(169, 64);
            label3.Name = "label3";
            label3.Size = new Size(138, 32);
            label3.TabIndex = 7;
            label3.Text = "by JamGrif";
            // 
            // LauncherForm
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(496, 548);
            Controls.Add(label3);
            Controls.Add(YouTube);
            Controls.Add(GitHub);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(SceneSelectListBox);
            Controls.Add(LaunchButton);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Icon = (Icon)resources.GetObject("$this.Icon");
            Margin = new Padding(3, 4, 3, 4);
            MaximizeBox = false;
            Name = "LauncherForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Curve3D Launcher";
            Load += LauncherForm_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button LaunchButton;
        private ListBox SceneSelectListBox;
        private Label label1;
        private Label label2;
        private LinkLabel GitHub;
        private LinkLabel YouTube;
        private Label label3;
    }
}
