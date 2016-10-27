#include "ribi_qtmaindialog.h"

#include <cassert>
#include <stdexcept>
#include <string>

#include "file_to_vector.h"
#include "is_regular_file.h"
#include "ribi_parameters.h"
#include "ribi_results.h"
#include "ribi_simulation.h"
#include "ribi_menu_dialog.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDebug>
#include "ui_ribi_qtmaindialog.h"
#pragma GCC diagnostic pop

ribi::qtmaindialog::qtmaindialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ribi_qtmaindialog)
{
  ui->setupUi(this);
  on_button_clicked();
}

ribi::qtmaindialog::~qtmaindialog()
{
  delete ui;
}

void ribi::qtmaindialog::delete_old_files(const parameters& p)
{
  const auto filenames = {
    get_filename_dot(p.get_filename_genotype_frequency_graph()),
    get_filename_png(p.get_filename_genotype_frequency_graph())
  };
  for (const auto& filename: filenames)
  {
    if (is_regular_file(filename))
    {
      std::remove(filename.c_str());
    }
    assert(!is_regular_file(filename));
  }
}

ribi::parameters ribi::qtmaindialog::get_parameters() const
{
  const int max_genetic_distance{
    std::stoi(
      ui->parameters->item(0,0)->text().toStdString()
    )
  };
  const int n_generations{
    std::stoi(
      ui->parameters->item(1,0)->text().toStdString()
    )
  };
  const int n_pin_loci{
    std::stoi(
      ui->parameters->item(2,0)->text().toStdString()
    )
  };
  const int n_sil_loci{
    std::stoi(
      ui->parameters->item(3,0)->text().toStdString()
    )
  };
  const double pin_mutation_rate{
    std::stod(
      ui->parameters->item(4,0)->text().toStdString()
    )
  };
  const int population_size{
    std::stoi(
      ui->parameters->item(5,0)->text().toStdString()
    )
  };
  const std::string& rgfgraph_filename{
    ui->parameters->item(6,0)->text().toStdString()
  };
  const int rng_seed{
    std::stoi(
      ui->parameters->item(7,0)->text().toStdString()
    )
  };
  const int sampling_interval{
    std::stoi(
      ui->parameters->item(8,0)->text().toStdString()
    )
  };
  const double sil_mutation_rate{
    std::stod(
      ui->parameters->item(9,0)->text().toStdString()
    )
  };

  return parameters(
    max_genetic_distance,
    n_generations,
    static_cast<std::size_t>(n_pin_loci),
    static_cast<std::size_t>(n_sil_loci),
    pin_mutation_rate,
    population_size,
    rgfgraph_filename, //results_genotype_frequency_graph_filename
    rng_seed,
    sampling_interval,
    sil_mutation_rate
  );
}

void ribi::qtmaindialog::on_button_clicked()
{
  const auto p = get_parameters();
  delete_old_files(p);
  try
  {
    menu_dialog d;
    d.run(p);
  }
  catch (std::exception& e)
  {
    ui->result->setText(e.what());

  }
  show_results(p);
}

void ribi::qtmaindialog::on_load_clicked()
{
  switch (ui->parameter_index->value())
  {
    case 1: set_parameters(create_test_parameters_1()); break;
    case 2: set_parameters(create_test_parameters_2()); break;
    default:
      qDebug() << "parameter_index not implemented";
  }
}

void ribi::qtmaindialog::set_parameters(const parameters& p) const
{
  ui->parameters->item(0,0)->setText(
    std::to_string(p.get_max_genetic_distance()).c_str()
  );
  ui->parameters->item(1,0)->setText(
    std::to_string(p.get_n_generations()).c_str()
  );
  ui->parameters->item(2,0)->setText(
    std::to_string(p.get_n_pin_loci()).c_str()
  );
  ui->parameters->item(3,0)->setText(
    std::to_string(p.get_n_sil_loci()).c_str()
  );
  ui->parameters->item(4,0)->setText(
    std::to_string(p.get_pin_mutation_rate()).c_str()
  );
  ui->parameters->item(5,0)->setText(
    std::to_string(p.get_population_size()).c_str()
  );
  ui->parameters->item(6,0)->setText(
    p.get_filename_genotype_frequency_graph().c_str()
  );
  ui->parameters->item(7,0)->setText(
    std::to_string(p.get_rng_seed()).c_str()
  );
  ui->parameters->item(8,0)->setText(
    std::to_string(p.get_sampling_interval()).c_str()
  );
  ui->parameters->item(9,0)->setText(
    std::to_string(p.get_sil_mutation_rate()).c_str()
  );
}

void ribi::qtmaindialog::show_results(const parameters& p)
{
  //Try to show the picture
  const std::string png_filename{
    get_filename_png(p.get_filename_genotype_frequency_graph())
  };
  if (is_regular_file(png_filename))
  {
    QPixmap pixmap;
    pixmap.load(png_filename.c_str());
    ui->result->setPixmap(pixmap);
    return;
  }
  //Try to show a .dot file
  const std::string dot_filename{
    get_filename_dot(p.get_filename_genotype_frequency_graph())
  };
  if (is_regular_file(dot_filename))
  {
    const auto text = file_to_vector(dot_filename);
    std::string s;
    for (const auto& t: text) { s += t + '\n'; }
    ui->result->setText(s.c_str());
  }
}
