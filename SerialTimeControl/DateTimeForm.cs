using System.IO.Ports;
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualBasic;

namespace SerialTimeControl
{
    public partial class DateTimeForm : Form
    {
        SerialPort serialPort = new()
        {
            BaudRate = 115200,
            DataBits = 8,
            StopBits = StopBits.One,
            Parity = Parity.None,
            PortName = "COM3",
        };

        const string DATE_FORMAT = "dd:MM - HH:mm:ss";

        public DateTimeForm()
        {
            InitializeComponent();
            dateTimePicker.Format = DateTimePickerFormat.Custom;
            dateTimePicker.CustomFormat = DATE_FORMAT;
            dateTimePicker.Value = DateTime.Now;
        }

        Task? transferTask;

        private void submit_Click(object sender, EventArgs e)
        {
            transferTask ??= transferDateAndTime();
        }

        private void textBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                transferTask ??= transferDateAndTime();
                e.Handled = true;
                e.SuppressKeyPress = true;
            }
        }

        private void dateTimePicker_ValueChanged(object sender, EventArgs e)
        {
            textBox.Text = dateTimePicker.Value.ToString(DATE_FORMAT);
            if (updateOnChangeCheckbox.Checked)
            {
                transferTask ??= transferDateAndTime();
            }
        }



        async Task transferDateAndTime()
        {
            if (transferTask != null && !transferTask.IsCompleted)
                return;

            if (!serialPort.IsOpen)
                serialPort.Open();

            byte[] buffer = Encoding.Default.GetBytes(textBox.Text);

            for (int i = 0; i < buffer.Length; i++)
            {
                await Task.Run(() =>
                {
                    serialPort.Write(buffer, i, 1);
                    Thread.Sleep(1);
                });
            }

            transferTask = null;
        }

    }
}