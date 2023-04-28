namespace SerialTimeControl
{
    partial class DateTimeForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            textBox = new TextBox();
            submit = new Button();
            dateTimePicker = new DateTimePicker();
            updateOnChangeCheckbox = new CheckBox();
            SuspendLayout();
            // 
            // textBox
            // 
            textBox.Location = new Point(36, 61);
            textBox.Name = "textBox";
            textBox.Size = new Size(213, 23);
            textBox.TabIndex = 0;
            textBox.Text = "12:00 - 09:55:21";
            textBox.KeyDown += textBox_KeyDown;
            // 
            // submit
            // 
            submit.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            submit.Location = new Point(162, 138);
            submit.Name = "submit";
            submit.Size = new Size(87, 38);
            submit.TabIndex = 1;
            submit.Text = "Datum ändern";
            submit.UseVisualStyleBackColor = true;
            submit.Click += submit_Click;
            // 
            // dateTimePicker
            // 
            dateTimePicker.Location = new Point(36, 32);
            dateTimePicker.Name = "dateTimePicker";
            dateTimePicker.Size = new Size(213, 23);
            dateTimePicker.TabIndex = 2;
            dateTimePicker.ValueChanged += dateTimePicker_ValueChanged;
            // 
            // updateOnChangeCheckbox
            // 
            updateOnChangeCheckbox.AutoSize = true;
            updateOnChangeCheckbox.Location = new Point(36, 99);
            updateOnChangeCheckbox.Name = "updateOnChangeCheckbox";
            updateOnChangeCheckbox.Size = new Size(123, 19);
            updateOnChangeCheckbox.TabIndex = 3;
            updateOnChangeCheckbox.Text = "Update on change";
            updateOnChangeCheckbox.UseVisualStyleBackColor = true;
            // 
            // DateTimeForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(290, 188);
            Controls.Add(updateOnChangeCheckbox);
            Controls.Add(dateTimePicker);
            Controls.Add(submit);
            Controls.Add(textBox);
            MaximizeBox = false;
            MinimizeBox = false;
            Name = "DateTimeForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Datum und Uhrzeit auswählen";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox textBox;
        private Button submit;
        private DateTimePicker dateTimePicker;
        private CheckBox updateOnChangeCheckbox;
    }
}