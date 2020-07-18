namespace MachanismAnalysis.GUI
{
    partial class PointWatchForm
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
            this.pgMain = new System.Windows.Forms.PropertyGrid();
            this.SuspendLayout();
            // 
            // pgMain
            // 
            this.pgMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pgMain.Location = new System.Drawing.Point(0, 0);
            this.pgMain.Name = "pgMain";
            this.pgMain.Size = new System.Drawing.Size(295, 396);
            this.pgMain.TabIndex = 0;
            // 
            // PointWatchForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(295, 396);
            this.Controls.Add(this.pgMain);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "PointWatchForm";
            this.Opacity = 0.7D;
            this.ShowInTaskbar = false;
            this.Text = "监视";
            this.GotFocus += PointWatchForm_GotFocus;
            this.ResumeLayout(false);

        }

        

        #endregion

        private System.Windows.Forms.PropertyGrid pgMain;
    }
}