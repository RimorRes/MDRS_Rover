namespace @interface
{
    partial class Rover_Interface
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Rover_Interface));
            this.panel1 = new System.Windows.Forms.Panel();
            this.button2 = new System.Windows.Forms.Button();
            this.Settings_button = new System.Windows.Forms.Button();
            this.Settings_groupbox = new System.Windows.Forms.GroupBox();
            this.Communication_status = new System.Windows.Forms.Label();
            this.port_comboBox = new System.Windows.Forms.ComboBox();
            this.Connectivity_label = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.Settings_groupbox.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(24)))), ((int)(((byte)(30)))), ((int)(((byte)(54)))));
            this.panel1.Controls.Add(this.button2);
            this.panel1.Controls.Add(this.Settings_button);
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(136, 445);
            this.panel1.TabIndex = 0;
            // 
            // button2
            // 
            this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button2.FlatAppearance.BorderSize = 0;
            this.button2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button2.Font = new System.Drawing.Font("Nirmala UI", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(126)))), ((int)(((byte)(249)))));
            this.button2.Location = new System.Drawing.Point(0, 395);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(136, 50);
            this.button2.TabIndex = 2;
            this.button2.Text = "Quit";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // Settings_button
            // 
            this.Settings_button.FlatAppearance.BorderSize = 0;
            this.Settings_button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Settings_button.Font = new System.Drawing.Font("Nirmala UI", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Settings_button.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(126)))), ((int)(((byte)(249)))));
            this.Settings_button.Location = new System.Drawing.Point(0, 0);
            this.Settings_button.Name = "Settings_button";
            this.Settings_button.Size = new System.Drawing.Size(136, 50);
            this.Settings_button.TabIndex = 1;
            this.Settings_button.Text = "Settings";
            this.Settings_button.UseVisualStyleBackColor = true;
            this.Settings_button.Click += new System.EventHandler(this.button1_Click);
            // 
            // Settings_groupbox
            // 
            this.Settings_groupbox.Controls.Add(this.Communication_status);
            this.Settings_groupbox.Controls.Add(this.port_comboBox);
            this.Settings_groupbox.Controls.Add(this.Connectivity_label);
            this.Settings_groupbox.Font = new System.Drawing.Font("Nirmala UI", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Settings_groupbox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(126)))), ((int)(((byte)(249)))));
            this.Settings_groupbox.Location = new System.Drawing.Point(142, 12);
            this.Settings_groupbox.Name = "Settings_groupbox";
            this.Settings_groupbox.Size = new System.Drawing.Size(324, 420);
            this.Settings_groupbox.TabIndex = 1;
            this.Settings_groupbox.TabStop = false;
            this.Settings_groupbox.Text = "Setttings";
            // 
            // Communication_status
            // 
            this.Communication_status.AutoSize = true;
            this.Communication_status.Font = new System.Drawing.Font("Nirmala UI", 8F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Communication_status.Location = new System.Drawing.Point(151, 36);
            this.Communication_status.Name = "Communication_status";
            this.Communication_status.Size = new System.Drawing.Size(148, 21);
            this.Communication_status.TabIndex = 2;
            this.Communication_status.Text = "Searching for Rover";
            // 
            // port_comboBox
            // 
            this.port_comboBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(46)))), ((int)(((byte)(51)))), ((int)(((byte)(73)))));
            this.port_comboBox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(126)))), ((int)(((byte)(249)))));
            this.port_comboBox.FormattingEnabled = true;
            this.port_comboBox.Items.AddRange(new object[] {
            "Auto",
            "None"});
            this.port_comboBox.Location = new System.Drawing.Point(6, 61);
            this.port_comboBox.Name = "port_comboBox";
            this.port_comboBox.Size = new System.Drawing.Size(312, 36);
            this.port_comboBox.TabIndex = 1;
            this.port_comboBox.Text = "Port";
            this.port_comboBox.SelectionChangeCommitted += new System.EventHandler(this.port_comboBox_SelectionChangeCommitted);
            // 
            // Connectivity_label
            // 
            this.Connectivity_label.AutoSize = true;
            this.Connectivity_label.Font = new System.Drawing.Font("Nirmala UI", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Connectivity_label.Location = new System.Drawing.Point(6, 30);
            this.Connectivity_label.Name = "Connectivity_label";
            this.Connectivity_label.Size = new System.Drawing.Size(151, 28);
            this.Connectivity_label.TabIndex = 0;
            this.Connectivity_label.Text = "Communication";
            // 
            // Rover_Interface
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(46)))), ((int)(((byte)(51)))), ((int)(((byte)(73)))));
            this.ClientSize = new System.Drawing.Size(478, 444);
            this.Controls.Add(this.Settings_groupbox);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Rover_Interface";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Rover Interface";
            this.panel1.ResumeLayout(false);
            this.Settings_groupbox.ResumeLayout(false);
            this.Settings_groupbox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button Settings_button;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.GroupBox Settings_groupbox;
        private System.Windows.Forms.Label Connectivity_label;
        private System.Windows.Forms.ComboBox port_comboBox;
        private System.Windows.Forms.Label Communication_status;
    }
}

